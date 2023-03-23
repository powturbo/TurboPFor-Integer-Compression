/*
 *  test for OPT-pfd
 *
 *      Author: sding
 *
 *
 */



#include<iostream>
#include <stdlib.h>
#include <stdio.h>

#include "opt_p4.h"

using namespace std;

char PATH[128] = "/usr/home/shuai/dumplist/wordlist_Excite";		// for reading list

int get_list(char *term, unsigned int *doc_id, unsigned int *freq, unsigned int *maxc)
{
	char fpath[128];
	sprintf(fpath,"%s/%s",PATH,term);
	FILE *fdd = fopen(fpath,"r");
	if(fdd==NULL)	return 0;

	int nread, npos;

	nread = fread(&npos, sizeof(unsigned), 1, fdd);
	npos = 0;

	while (nread > 0)
	{
		nread = fread(&doc_id[npos], sizeof(unsigned), 1, fdd);
		if (nread <= 0)  break;
		fread(&freq[npos], sizeof(unsigned), 1, fdd);
		npos++;
	}
	fclose(fdd);

	int i;

    /* fill out the max values */
	for (i = 0; i < npos; i += BS)
		maxc[(i/BS)] = doc_id[i+BS-1];

    /* take the gap for doc_id */
	for (i = npos-1; i > 0; i--)
	{	
		doc_id[i] -= doc_id[i-1];
		doc_id[i] --;
	}

	for (i = 0; i < npos; i++)
		freq[i]--;
	return npos;
}

int main()	// just for testing
{
	int MAX_NDOC = 25205179;
	unsigned int *docid = new unsigned int[MAX_NDOC];
	unsigned int *docid_check = new unsigned int[MAX_NDOC ];

	unsigned int *fre = new unsigned int[MAX_NDOC];
	unsigned int *maxc = new unsigned int[MAX_NDOC/BS];
	unsigned int *aux = new unsigned int[MAX_NDOC];
	unsigned int * all_array = new unsigned int[2048];		// extra array for coding
	

	int listSize = get_list("information", docid, fre, maxc);
	cout<<"list size is "<<listSize<<endl;
	for(int i=0;i<listSize; i++)
		docid_check[i] = docid[i];

	int cSize = OPT4(docid,listSize,aux);
	cout<<"Compressed size is "<<cSize<<" byte"<<endl;

	unsigned int *_ww = aux;
	for (int i = 0; i*BS < listSize; i++)
	{
		/* _ww = detailed_p4_decode(docid_check, _ww, all_array); */		// this is fast
		_ww = 	detailed_p4_decode(docid_check + BS * i, _ww,  all_array);	// this is slow
	}
	// check correctness
	for(int i=0;i<listSize;i++)
	{
		if( docid_check[i]!= docid[i] )
		{
			cout<<"Exceptions happen"<<endl;
			exit (1);
		}
	}
	delete []docid;
	delete []docid_check;
	delete []fre;
	delete []maxc;
	delete []aux;
	delete []all_array;
}
