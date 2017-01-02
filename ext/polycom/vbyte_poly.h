// 
#define VBYTE_ENC(_v, _n)	\
{\
	unsigned _num;				\
	unsigned char _barray[5];	\
	unsigned _i, _started = 0;	\
 	_num = _n;					\
	for (_i = 0; _i < 5; _i++)	\
	{							\
		_barray[_i] = ((_num%128)<<1);	\
		_num = _num/128;		\
	}							\
	for (_i = 4; _i > 0; _i--)	\
	{							\
		if ((_barray[_i] != 0) || (_started == 1))	\
		{						\
			_started = 1;		\
			*_v = _barray[_i]|0x1;	\
			_v++;				\
		}						\
	}							\
	*_v = _barray[0]|0x0;		\
	_v++;						\
}

#define VBYTE_DEC(_v, _n)	\
{\
	_n = ((*_v>>1));						\
	if ((*_v&0x1) != 0)		\
        {							\
          _v++;				\
	  _n = (_n<<7) + ((*_v>>1));	\
	  if ((*_v&0x1)!= 0)		\
          {						\
            _v++;				\
	    _n = (_n<<7) + ((*_v>>1));	\
	    if ((*_v&0x1) != 0)		\
            {						\
              _v++;				\
	      _n = (_n<<7) + ((*_v>>1));	\
	    }\
	  }\
	}\
        _v++;				\
}

