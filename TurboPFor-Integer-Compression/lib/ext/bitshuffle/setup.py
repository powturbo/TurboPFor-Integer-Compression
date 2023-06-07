from __future__ import absolute_import, division, print_function

# I didn't import unicode_literals. They break setuptools or Cython in python
# 2.7, but python 3 seems to be happy with them.

import glob
import os
from os import path
from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext as build_ext_
from setuptools.command.develop import develop as develop_
from setuptools.command.install import install as install_
from Cython.Compiler.Main import default_options
import shutil
import subprocess
import sys
import platform


VERSION_MAJOR = 0
VERSION_MINOR = 5
VERSION_POINT = 1
# Define ZSTD macro for cython compilation
default_options["compile_time_env"] = {"ZSTD_SUPPORT": False}

# Only unset in the 'release' branch and in tags.
VERSION_DEV = None

VERSION = "%d.%d.%d" % (VERSION_MAJOR, VERSION_MINOR, VERSION_POINT)
if VERSION_DEV:
    VERSION = VERSION + ".dev%d" % VERSION_DEV


COMPILE_FLAGS = ["-O3", "-ffast-math", "-std=c99"]
# Cython breaks strict aliasing rules.
COMPILE_FLAGS += ["-fno-strict-aliasing"]
COMPILE_FLAGS += ["-fPIC"]
COMPILE_FLAGS_MSVC = ["/Ox", "/fp:fast"]

MACROS = [
    ("BSHUF_VERSION_MAJOR", VERSION_MAJOR),
    ("BSHUF_VERSION_MINOR", VERSION_MINOR),
    ("BSHUF_VERSION_POINT", VERSION_POINT),
]


H5PLUGINS_DEFAULT = "/usr/local/hdf5/lib/plugin"

# OSX's clang compiler does not support OpenMP.
if sys.platform == "darwin":
    OMP_DEFAULT = False
else:
    OMP_DEFAULT = True

# Build against the native architecture unless overridden by an environment variable
# This can also be overridden by a direct command line argument, or a `setup.cfg` entry
# This option is needed for the cibuildwheel action
if "BITSHUFFLE_ARCH" in os.environ:
    MARCH_DEFAULT = os.environ["BITSHUFFLE_ARCH"]
else:
    MARCH_DEFAULT = "native"

FALLBACK_CONFIG = {
    "include_dirs": [],
    "library_dirs": [],
    "libraries": [],
    "extra_compile_args": [],
    "extra_link_args": [],
}

if "HDF5_DIR" in os.environ:
    FALLBACK_CONFIG["include_dirs"] += [os.environ["HDF5_DIR"] + "/include"]  # macports
    FALLBACK_CONFIG["library_dirs"] += [os.environ["HDF5_DIR"] + "/lib"]  # macports
elif sys.platform == "darwin":
    # putting here both macports and homebrew paths will generate
    # "ld: warning: dir not found" at the linking phase
    FALLBACK_CONFIG["include_dirs"] += ["/opt/local/include"]  # macports
    FALLBACK_CONFIG["library_dirs"] += ["/opt/local/lib"]  # macports
    FALLBACK_CONFIG["include_dirs"] += ["/usr/local/include"]  # homebrew
    FALLBACK_CONFIG["library_dirs"] += ["/usr/local/lib"]  # homebrew
elif sys.platform.startswith("freebsd"):
    FALLBACK_CONFIG["include_dirs"] += ["/usr/local/include"]  # homebrew
    FALLBACK_CONFIG["library_dirs"] += ["/usr/local/lib"]  # homebrew

FALLBACK_CONFIG["include_dirs"] = [
    d for d in FALLBACK_CONFIG["include_dirs"] if path.isdir(d)
]
FALLBACK_CONFIG["library_dirs"] = [
    d for d in FALLBACK_CONFIG["library_dirs"] if path.isdir(d)
]

FALLBACK_CONFIG["extra_compile_args"] = ["-DH5_BUILT_AS_DYNAMIC_LIB"]


def pkgconfig(*packages, **kw):
    config = kw.setdefault("config", {})
    optional_args = kw.setdefault("optional", "")
    flag_map = {
        "include_dirs": ["--cflags-only-I", 2],
        "library_dirs": ["--libs-only-L", 2],
        "libraries": ["--libs-only-l", 2],
        "extra_compile_args": ["--cflags-only-other", 0],
        "extra_link_args": ["--libs-only-other", 0],
    }
    for package in packages:
        try:
            subprocess.check_output(["pkg-config", package])
        except (subprocess.CalledProcessError, OSError):
            print(
                "Can't find %s with pkg-config fallback to " "static config" % package
            )
            for distutils_key in flag_map:
                config.setdefault(distutils_key, []).extend(
                    FALLBACK_CONFIG[distutils_key]
                )
            config["libraries"].append(package)
        else:
            for distutils_key, (pkg_option, n) in flag_map.items():
                items = (
                    subprocess.check_output(
                        ["pkg-config", optional_args, pkg_option, package]
                    )
                    .decode("utf8")
                    .split()
                )
                opt = config.setdefault(distutils_key, [])
                opt.extend([i[n:] for i in items])
    return config


zstd_headers = ["zstd/lib/zstd.h"]
zstd_lib = ["zstd/lib/"]
zstd_sources = glob.glob("zstd/lib/common/*.c")
zstd_sources += glob.glob("zstd/lib/compress/*.c")
zstd_sources += glob.glob("zstd/lib/decompress/*.c")

ext_bshuf = Extension(
    "bitshuffle.ext",
    sources=[
        "bitshuffle/ext.pyx",
        "src/bitshuffle.c",
        "src/bitshuffle_core.c",
        "src/iochain.c",
        "lz4/lz4.c",
    ],
    include_dirs=["src/", "lz4/"],
    depends=["src/bitshuffle.h", "src/bitshuffle_core.h", "src/iochain.h", "lz4/lz4.h"],
    libraries=[],
    define_macros=MACROS,
)

h5filter = Extension(
    "bitshuffle.h5",
    sources=[
        "bitshuffle/h5.pyx",
        "src/bshuf_h5filter.c",
        "src/bitshuffle.c",
        "src/bitshuffle_core.c",
        "src/iochain.c",
        "lz4/lz4.c",
    ],
    depends=[
        "src/bitshuffle.h",
        "src/bitshuffle_core.h",
        "src/iochain.h",
        "src/bshuf_h5filter.h",
        "lz4/lz4.h",
    ],
    define_macros=MACROS + [("H5_USE_18_API", None)],
    **pkgconfig("hdf5", config=dict(include_dirs=["src/", "lz4/"]))
)

if not sys.platform.startswith("win"):
    h5filter.sources.append("src/hdf5_dl.c")
    h5filter.libraries.remove("hdf5")

filter_plugin = Extension(
    "bitshuffle.plugin.libh5bshuf",
    sources=[
        "src/bshuf_h5plugin.c",
        "src/bshuf_h5filter.c",
        "src/bitshuffle.c",
        "src/bitshuffle_core.c",
        "src/iochain.c",
        "lz4/lz4.c",
    ],
    depends=[
        "src/bitshuffle.h",
        "src/bitshuffle_core.h",
        "src/iochain.h",
        "src/bshuf_h5filter.h",
        "lz4/lz4.h",
    ],
    define_macros=MACROS,
    **pkgconfig("hdf5", config=dict(include_dirs=["src/", "lz4/"]))
)

lzf_plugin = Extension(
    "bitshuffle.plugin.libh5LZF",
    sources=[
        "src/lzf_h5plugin.c",
        "lzf/lzf_filter.c",
        "lzf/lzf/lzf_c.c",
        "lzf/lzf/lzf_d.c",
    ],
    depends=["lzf/lzf_filter.h", "lzf/lzf/lzf.h", "lzf/lzf/lzfP.h"],
    **pkgconfig("hdf5", config=dict(include_dirs=["lzf/", "lzf/lzf/"]))
)


EXTENSIONS = [
    ext_bshuf,
]

# Check for HDF5 support
HDF5_FILTER_SUPPORT = False
CPATHS = os.environ["CPATH"].split(":") if "CPATH" in os.environ else []
for p in ["/usr/include"] + pkgconfig("hdf5")["include_dirs"] + CPATHS:
    if os.path.exists(os.path.join(p, "hdf5.h")):
        HDF5_FILTER_SUPPORT = True

if HDF5_FILTER_SUPPORT:
    EXTENSIONS.append(h5filter)

# Check for plugin hdf5 plugin support (hdf5 >= 1.8.11)
HDF5_PLUGIN_SUPPORT = False
CPATHS = os.environ["CPATH"].split(":") if "CPATH" in os.environ else []
for p in ["/usr/include"] + pkgconfig("hdf5")["include_dirs"] + CPATHS:
    if os.path.exists(os.path.join(p, "H5PLextern.h")):
        HDF5_PLUGIN_SUPPORT = True

if HDF5_PLUGIN_SUPPORT:
    EXTENSIONS.extend([filter_plugin, lzf_plugin])

# For enabling ZSTD support when building wheels
# This needs to be done after all Extensions have been added to EXTENSIONS
if "ENABLE_ZSTD" in os.environ:
    default_options["compile_time_env"] = {"ZSTD_SUPPORT": True}
    for ext in EXTENSIONS:
        if ext.name in [
            "bitshuffle.ext",
            "bitshuffle.h5",
            "bitshuffle.plugin.libh5bshuf",
        ]:
            ext.sources += zstd_sources
            ext.include_dirs += zstd_lib
            ext.depends += zstd_headers
            ext.define_macros += [("ZSTD_SUPPORT", 1)]


class develop(develop_):
    def run(self):
        # Dummy directory for copying build plugins.
        if not path.isdir("bitshuffle/plugin"):
            os.mkdir("bitshuffle/plugin")
        develop_.run(self)


# Custom installation to include installing dynamic filters.
class install(install_):
    user_options = install_.user_options + [
        ("h5plugin", None, "Install HDF5 filter plugins for use outside of python."),
        (
            "h5plugin-dir=",
            None,
            "Where to install filter plugins. Default %s." % H5PLUGINS_DEFAULT,
        ),
        ("zstd", None, "Install ZSTD support."),
    ]

    def initialize_options(self):
        install_.initialize_options(self)
        self.h5plugin = False
        self.zstd = False
        self.h5plugin_dir = H5PLUGINS_DEFAULT

    def finalize_options(self):
        install_.finalize_options(self)
        if self.h5plugin not in ("0", "1", True, False):
            raise ValueError("Invalid h5plugin argument. Must be '0' or '1'.")
        self.h5plugin = int(self.h5plugin)
        self.h5plugin_dir = path.abspath(self.h5plugin_dir)
        self.zstd = self.zstd

        # Add ZSTD files and macro to extensions if ZSTD enabled
        if self.zstd:
            default_options["compile_time_env"] = {"ZSTD_SUPPORT": True}
            for ext in EXTENSIONS:
                if ext.name in [
                    "bitshuffle.ext",
                    "bitshuffle.h5",
                    "bitshuffle.plugin.libh5bshuf",
                ]:
                    ext.sources += zstd_sources
                    ext.include_dirs += zstd_lib
                    ext.depends += zstd_headers
                    ext.define_macros += [("ZSTD_SUPPORT", 1)]

    def run(self):
        install_.run(self)
        if self.h5plugin:
            if not HDF5_PLUGIN_SUPPORT:
                print("HDF5 < 1.8.11, not installing filter plugins.")
                return
            plugin_build = path.join(self.build_lib, "bitshuffle", "plugin")
            try:
                os.makedirs(self.h5plugin_dir)
            except OSError as e:
                if e.args[0] == 17:
                    # Directory already exists, this is fine.
                    pass
                else:
                    raise
            plugin_libs = glob.glob(path.join(plugin_build, "*"))
            for plugin_lib in plugin_libs:
                plugin_name = path.split(plugin_lib)[1]
                shutil.copy2(plugin_lib, path.join(self.h5plugin_dir, plugin_name))
            print("Installed HDF5 filter plugins to %s" % self.h5plugin_dir)


# Command line or site.cfg specification of OpenMP.
class build_ext(build_ext_):
    user_options = build_ext_.user_options + [
        (
            "omp=",
            None,
            "Whether to compile with OpenMP threading. Default"
            " on current system is %s." % str(OMP_DEFAULT),
        ),
        (
            "march=",
            None,
            "Generate instructions for a specific machine type. Default is %s."
            % MARCH_DEFAULT,
        ),
    ]
    boolean_options = build_ext_.boolean_options + ["omp"]

    def initialize_options(self):
        build_ext_.initialize_options(self)
        self.omp = OMP_DEFAULT
        self.march = MARCH_DEFAULT

    def finalize_options(self):
        # For some reason this gets run twice. Careful to print messages and
        # add arguments only one time.
        build_ext_.finalize_options(self)

        if self.omp not in ("0", "1", True, False):
            raise ValueError("Invalid omp argument. Mut be '0' or '1'.")
        self.omp = int(self.omp)

        import numpy as np

        ext_bshuf.include_dirs.append(np.get_include())

        # Required only by old version of setuptools < 18.0
        from Cython.Build import cythonize

        self.extensions = cythonize(self.extensions)
        for ext in self.extensions:
            ext._needs_stub = False

    def build_extensions(self):
        c = self.compiler.compiler_type

        # Set compiler flags including architecture
        if self.compiler.compiler_type == "msvc":
            openmpflag = "/openmp"
            compileflags = COMPILE_FLAGS_MSVC
        else:
            openmpflag = "-fopenmp"
            archi = platform.machine()
            if archi in ("i386", "x86_64"):
                compileflags = COMPILE_FLAGS + ["-march=%s" % self.march]
            else:
                compileflags = COMPILE_FLAGS + ["-mcpu=%s" % self.march]
                if archi == "ppc64le":
                    compileflags = COMPILE_FLAGS + ["-DNO_WARN_X86_INTRINSICS"]

        if self.omp not in ("0", "1", True, False):
            raise ValueError("Invalid omp argument. Mut be '0' or '1'.")
        self.omp = int(self.omp)

        # Add the appropriate OpenMP flags if needed
        if self.omp:
            if not hasattr(self, "_printed_omp_message"):
                self._printed_omp_message = True
                print("\n#################################")
                print("# Compiling with OpenMP support #")
                print("#################################\n")
            # More portable to pass -fopenmp to linker.
            # self.libraries += ['gomp']
            compileflags += [openmpflag]
            linkflags = [openmpflag]
        else:
            linkflags = []

        # Add the compile/link options to each extension
        for e in self.extensions:
            e.extra_compile_args = list(set(e.extra_compile_args).union(compileflags))
            e.extra_link_args = list(set(e.extra_link_args).union(linkflags))

        build_ext_.build_extensions(self)


# Don't install numpy/cython/hdf5 if not needed
for cmd in ["sdist", "clean", "--help", "--help-commands", "--version"]:
    if cmd in sys.argv:
        setup_requires = []
        break
else:
    setup_requires = ["Cython>=0.19", "numpy>=1.6.1"]

with open("requirements.txt") as f:
    requires = f.read().splitlines()
    requires = [r.split()[0] for r in requires]

with open("README.rst") as r:
    long_description = r.read()

# TODO hdf5 support should be an "extra". Figure out how to set this up.
setup(
    name="bitshuffle",
    version=VERSION,
    packages=["bitshuffle", "bitshuffle"],
    scripts=[],
    ext_modules=EXTENSIONS,
    cmdclass={"build_ext": build_ext, "install": install, "develop": develop},
    setup_requires=setup_requires,
    install_requires=requires,
    # extras_require={'H5':  ["h5py"]},
    package_data={"": ["data/*"]},
    # metadata for upload to PyPI
    author="Kiyoshi Wesley Masui",
    author_email="kiyo@physics.ubc.ca",
    description="Bitshuffle filter for improving typed data compression.",
    long_description=long_description,
    license="MIT",
    url="https://github.com/kiyo-masui/bitshuffle",
    download_url=("https://github.com/kiyo-masui/bitshuffle/tarball/%s" % VERSION),
    keywords=["compression", "hdf5", "numpy"],
)
