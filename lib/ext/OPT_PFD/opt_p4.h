#include "pf.h"
#define BS 128
//using namespace std;
//file "OPT_PFD.zip" form: http://jinruhe.com/  
//int dnum[17] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,16,20,32};

void p4_encode(unsigned int *doc_id, int npos, int b,unsigned int *buf , int *size, int *ex_n)
{
	int i = 0;
	unsigned int *ww = buf;
	detailed_p4_encode(&ww, &(doc_id[i]), b, size,ex_n);
}

/*
*	when list_size is too small, not good to use this function
*/
int OPT4(unsigned int *doc_id,unsigned int list_size,unsigned int *aux)
{
	int i,j,l;
	for(i=0; i<2*BS; i++)
	{
		doc_id[i+list_size] = 0 ;		// pack the input, avoid garbage data in the end
	}
	int size = 0;
	int ex_n = 0;
	int csize = 0; 	// compressed size in bytes
			
	int chunk_size = 0;																					
	int b = -1, temp_en = 0;
	int offset = 0;
	for(j=0;j<list_size;j+=BS)				// for each chunk
	{
		chunk_size = 999999999;
		b = -1;
		// get the smallest chunk size by trying all b's
		for(l=0;l<16;l++)
				{
					p4_encode(doc_id+j, BS, l, aux+offset, &size, &ex_n);
					if(chunk_size > size * 4)			// int bytes
					{
						chunk_size = size *4;
						b = l;
						temp_en = ex_n;
					}		
				}	

				csize += chunk_size;
				//printf("encode:%u\n", b);
				p4_encode(doc_id + j, BS, b, aux + offset, &size, &ex_n);
				offset += size;
	}
	
	return csize;	
}
