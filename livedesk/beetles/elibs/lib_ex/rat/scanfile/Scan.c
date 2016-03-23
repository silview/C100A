/*
*********************************************************************************************************
*                                                    ePDK
*                                   the Easy Portable/Player Development Kit
*                                       ebook information manager libary
*
*                                    (c) Copyright 2010-2012, Sunny China
*                                              All Rights Reserved
*
* File    : Scan.c
* By      : Sunny
* Version : V1.00
* Date    : 2010-4-22 13:54:03
*********************************************************************************************************
*/

#include	"OAL.h"
#include	"ScanPri.h"
#include    "rat_common.h"

#define LIST_DATA_OFFSET(list) 		((int)((list)->Data - (__u8 *)(list)))
#define ENTRY_NAME_OFFSET(entry) 	((int)((entry)->Name - (__u8 *)(entry)))

#if 0
#define DebugPrintf(...)    		(eLIBs_printf("MSG:L%d(%s):", __LINE__, __FILE__),                 \
							     eLIBs_printf(__VA_ARGS__)		)
#else
#define DebugPrintf(...) 
#endif


//ADD by hot.lee for filelist sort
#define SCAN_SORT 0

#define IS_NUM   1
#define IS_LETTER   2
#define IS_CHN   3
#define IS_OTHER   0
typedef ScanedEntry_t rat_entry_t;//to store rat entry information.
//---------------------------------
static int 	  __ScanAllFiles(ScanInfo_t *pScanInfo); 
static int	  __SetupScanProcess(ScanInfo_t *pScanInfo);
static void   __DestoryScanInfo (ScanInfo_t *pScanInfo);
__bool __ValidFileType(const char *file, __u64 Types, int *Format);
static int __InsertFileEntryToList(char *filePath, 
								   int format,
								   ScanInfo_t *pScanInfo);

prat_search_cb g_rat_search_cb = NULL;
void* g_rat_search_arg = NULL;
#if (SCAN_SORT == 1)
/*
********************************************************************************************
*Function	:          static __s32 ExtractFileName(char *FileName, char *FilePath)
*
*Description	:		??????????
*					
*Arguments  	: 		
*
*Return     	: 	
*
**********************************************************************************************
*/
static __s32 ExtractFileName(char *FileName, char *FilePath)
{
	char *pFileName;
	pFileName = eLIBs_strchrlast(FilePath, '\\');
	pFileName++;
	eLIBs_strcpy(FileName, pFileName);	
	
	return EPDK_OK;
}


/*
************************************************************************************************************************
*Function	:          static __s32 _RatGetListItemFileName(explr_list_para_t *list_para, __s32 ItemIndex, char *FilePath)
*
*Description	:		???????????????
*					
*Arguments  	: 		
*
*Return     	: 	
*
************************************************************************************************************************
*/
static __s32 _RatGetListItemFileName(HRAT rat_handle, __s32 ItemIndex, char *FileName)
{
	int ret;
	rat_entry_t ItemInfo;
	//char FilePath[1024];
	
	eLIBs_memset(&ItemInfo,0,sizeof(rat_entry_t));	
	
	ret = rat_get_item_info_by_index(rat_handle, ItemIndex, &ItemInfo);	
	if(ret == EPDK_FAIL)
	{
		__wrn("get file information form rat fail!!");
		return EPDK_FAIL;
	}		
	//eLIBs_strcpy(FileName, ItemInfo.Path);			
	//?????rat ??,???????eLIBs_strcpy,??????

	ExtractFileName(FileName, ItemInfo.Path);

	return EPDK_OK;
}

/*
************************************************************************************************************************
*Function	:         static __s32 RatGetListItemFileName(explr_list_para_t *list_para, __s32 ItemIndex, char *FileName)
*
*Description	:		????????????(?????,???)
*					
*Arguments  	: 		
*
*Return     	: 	
*
************************************************************************************************************************
*/
static __s32 RatGetListItemFileName(HRAT rat_handle, __s32 ItemIndex, char *FileName)
{
	int ret;
	char FilePath[RAT_MAX_FULL_PATH_LEN]= "";
	//eLIBs_memset(FilePath,0,sizeof(FilePath));
	if(rat_handle == NULL)
	{
		return EPDK_FAIL;
	}
	ret = _RatGetListItemFileName(rat_handle, ItemIndex, FileName);
	if(ret == EPDK_FAIL)
	{
		return EPDK_FAIL;
	}
	
	
	return EPDK_OK;
}

/****************************************************************************
*
* ??????,??????:
* ???????:
* ?????????,???????,???0;
* ????????,???1;
* ????????,???2;
* ????????,???3
*
****************************************************************************/
static  __inline __s32 file_item_name_class(char *name)
{
	/*if (*name<0)
		return 3;
	else */if ((*name>='a'&&*name<='z')||(*name>='A'&&*name<='Z'))
		return 2;
	else if(*name>='0'&&*name<='9')
	    return 1;
	else
		return 3;
}
/****************************************************************************
*
* ??????(?????1)???????
*
*return: 1<2: -1   1==2:0   1>2:1
*
****************************************************************************/
static __inline __s32 num_name_cmp(char *nam1,char *nam2)
{
    __s32 i=0,j=0;
	__s32 sum1=0,sum2=0;
	__s32 value1,value2;
	__s32 maxlen;
    __s32 len1,len2;
	char name1[1024],name2[1024];

	__wrn("nam1 = %s  \n",nam1);
	__wrn("nam2 = %s  \n",nam2);
	len1 = eLIBs_strlen(nam1);
	len2 = eLIBs_strlen(nam2);	

    //??"02the"?????????,?????i?
	//if(nam1[len1/2] < '0' || nam1[len1/2] > '9'
	//	|| nam2[len2/2] < '0' || nam2[len2/2] > '9')
	{
	    while(i < len1)
	    {
            if(nam1[i] >= '0' && nam1[i] <= '9' )
            {                
				sum1 = sum1*10 + nam1[i] - '0';
				i++;
			}
			else
			{
                break;
			}
		}
	    while(j < len2)
	    {
            if(nam2[j] >= '0' && nam2[j] <= '9' )
            {                
				sum2 = sum2*10 + nam2[j] - '0';
				j++;
			}
			else
			{
                break;
			}
		}
		__wrn("sum1 = %d sum2 = %d \n",sum1,sum2);
		if(sum1 < sum2)
		{
            return -1;
		}
		else if( sum1 > sum2)
		{
			return 1;
		}
		else
		{
		    //????,?????eg: 01 1 ,then 01 > 1
			if(i < j)
			{
				return -1;
			}
			else if( i > j)
			{
				return 1;
			}

		}			
	}

	return eLIBs_strcmp((nam1+i),(nam2+j));

}

static __inline __s32 rat_chn_name_cmp(char *nam1, char *nam2)
{
#if 0
	__u8 str1[128],str2[128];
	eLIBs_encoding_stream_convert(EPDK_CHARSET_ENM_UTF8,EPDK_CHARSET_ENM_GBK,nam1,3,str1,3);
	eLIBs_encoding_stream_convert(EPDK_CHARSET_ENM_UTF8,EPDK_CHARSET_ENM_GBK,nam2,3,str2,3);	
	return eLIBs_strcmp(str1,str2);
#else
	return eLIBs_strcmp(nam1,nam2);
#endif
}


static  __s32 filelist_compare(void *cur, void *item)
{
	__s32 type1,type2;
	__s32 ret;
	char *cur_name,*item_name;
	ScanFileEntry_t *pEntry1,*pEntry2;

	
	ScanFileList_t* pcur = (*(ScanFileList_t **)cur);
	ScanFileList_t* pitem = (*(ScanFileList_t **)item);


	pEntry1 = (ScanFileEntry_t *)(pcur->Data);
	pEntry2 = (ScanFileEntry_t *)(pitem->Data);

	cur_name = eLIBs_strchrlast(pEntry1->Name,'\\')+1;
	item_name = eLIBs_strchrlast(pEntry2->Name,'\\')+1;
	
#if 0	
	if(NULL == cur_name || NULL == item_name)
	{
		__wrn("invalid para...\n");
		return 0;
	}
	
	return eLIBs_stricmp(cur_name, item_name);
#endif	
	///////////////////////////////
	type1 = file_item_name_class(cur_name);
	type2 = file_item_name_class(item_name);

	__wrn("pcur->name = %s item->name = %s type1 = %d type2 = %d\n",cur_name,item_name,type1,type2);
	{
		if(IS_NUM == type1 && IS_NUM == type2)
		{
			ret = num_name_cmp( cur_name , item_name );
			__wrn("ret = %d \n",ret);
		}
		else if((type1 == IS_NUM || type1 == IS_LETTER) && (type2 == IS_CHN))
		{
			ret = -1;//<
		}
		else if(type1 == IS_NUM && (type2 ==IS_LETTER || type2 == IS_CHN))
		{
			ret = -1;//<
		}
		else if((type1 == IS_LETTER || type2 == IS_CHN) && type2 == IS_NUM)
		{
			ret = 1;//>
		}				
		else
		{
		    #if 0
			ret = rat_chn_name_cmp( cur_name , item_name );
			#else
			ret = rat_chn_name_cmp( pEntry1->uin_name, pEntry2->uin_name );
			#endif
		}
	}	

	return ret;

}


///start qsort

#define STKSIZ (8*sizeof(void*) - 2)
#define CUTOFF 8 
#define __SHORTSORT(lo, hi, width, comp, context) shortsort(lo, hi, width, comp);
#define __COMPARE(context, p1, p2) comp(p1, p2)
#define size_t int
static void  swap (
    char *a,
    char *b,
    size_t width
    )
{
    char tmp;

    if ( a != b )
        /* Do the swap one character at a time to avoid potential alignment
           problems. */
        while ( width-- ) {
            tmp = *a;
            *a++ = *b;
            *b++ = tmp;
        }
}

static void  shortsort (
    char *lo,
    char *hi,
    size_t width,
    int (*comp)(const void *, const void *)
    )
   /* __USE_CONTEXT */
{
    char *p, *max;

    /* Note: in assertions below, i and j are alway inside original bound of
       array to sort. */
     __wrn("..........\n");

    while (hi > lo) {
        /* A[i] <= A[j] for i <= j, j > hi */
        max = lo;
        for (p = lo+width; p <= hi; p += width) {
            /* A[i] <= A[max] for lo <= i < p */
            if (__COMPARE(context, p, max) > 0) {
                max = p;
            }
            /* A[i] <= A[max] for lo <= i <= p */
        }

        /* A[i] <= A[max] for lo <= i <= hi */

        swap(max, hi, width);

        /* A[i] <= A[hi] for i <= hi, so A[i] <= A[j] for i <= j, j >= hi */

        hi -= width;

        /* A[i] <= A[j] for i <= j, j > hi, loop top condition established */
    }
    /* A[i] <= A[j] for i <= j, j > lo, which implies A[i] <= A[j] for i < j,
       so array is sorted */
}


static void  qsort (
    void *base,
    size_t num,
    size_t width,
    int (*comp)(const void *, const void *)
    )
/* __USE_CONTEXT */
{
    char *lo, *hi;              /* ends of sub-array currently sorting */
    char *mid;                  /* points to middle of subarray */
    char *loguy, *higuy;        /* traveling pointers for partition step */
    size_t size;                /* size of the sub-array */
    char *lostk[STKSIZ], *histk[STKSIZ];
    int stkptr;                 /* stack for saving sub-array to be processed */

  	__wrn("num = %d \n",num);
    if (num < 2)
        return;                 /* nothing to do */

    stkptr = 0;                 /* initialize stack */

    lo = (char *)base;
    hi = (char *)base + width * (num-1);        /* initialize limits */

	__wrn("lo = %08x hi = %08x\n",*lo,*hi);
    /* this entry point is for pseudo-recursion calling: setting
       lo and hi and jumping to here is like recursion, but stkptr is
       preserved, locals aren't, so we preserve stuff on the stack */
recurse:;

    size = (hi - lo) / width + 1;        /* number of el's to sort */
	__wrn("size = %d\n",size);
    /* below a certain size, it is faster to use a O(n^2) sorting method */
    if (size <= CUTOFF) {
        __SHORTSORT(lo, hi, width, comp, context);
    }
    else {
        /* First we pick a partitioning element.  The efficiency of the
           algorithm demands that we find one that is approximately the median
           of the values, but also that we select one fast.  We choose the
           median of the first, middle, and last elements, to avoid bad
           performance in the face of already sorted data, or data that is made
           up of multiple sorted runs appended together.  Testing shows that a
           median-of-three algorithm provides better performance than simply
           picking the middle element for the latter case. */

        mid = lo + (size / 2) * width;      /* find middle element */

        /* Sort the first, middle, last elements into order */
        if (__COMPARE(context, lo, mid) > 0) {
            swap(lo, mid, width);
        }
        if (__COMPARE(context, lo, hi) > 0) {
            swap(lo, hi, width);
        }
        if (__COMPARE(context, mid, hi) > 0) {
            swap(mid, hi, width);
        }

        /* We now wish to partition the array into three pieces, one consisting
           of elements <= partition element, one of elements equal to the
           partition element, and one of elements > than it.  This is done
           below; comments indicate conditions established at every step. */

        loguy = lo;
        higuy = hi;

        /* Note that higuy decreases and loguy increases on every iteration,
           so loop must terminate. */
        for (;;) {
            /* lo <= loguy < hi, lo < higuy <= hi,
               A[i] <= A[mid] for lo <= i <= loguy,
               A[i] > A[mid] for higuy <= i < hi,
               A[hi] >= A[mid] */

            /* The doubled loop is to avoid calling comp(mid,mid), since some
               existing comparison funcs don't work when passed the same
               value for both pointers. */

            if (mid > loguy) {
                do  {
                    loguy += width;
                } while (loguy < mid && __COMPARE(context, loguy, mid) <= 0);
            }
            if (mid <= loguy) {
                do  {
                    loguy += width;
                } while (loguy <= hi && __COMPARE(context, loguy, mid) <= 0);
            }

            /* lo < loguy <= hi+1, A[i] <= A[mid] for lo <= i < loguy,
               either loguy > hi or A[loguy] > A[mid] */

            do  {
                higuy -= width;
            } while (higuy > mid && __COMPARE(context, higuy, mid) > 0);

            /* lo <= higuy < hi, A[i] > A[mid] for higuy < i < hi,
               either higuy == lo or A[higuy] <= A[mid] */

            if (higuy < loguy)
                break;

            /* if loguy > hi or higuy == lo, then we would have exited, so
               A[loguy] > A[mid], A[higuy] <= A[mid],
               loguy <= hi, higuy > lo */

            swap(loguy, higuy, width);

            /* If the partition element was moved, follow it.  Only need
               to check for mid == higuy, since before the swap,
               A[loguy] > A[mid] implies loguy != mid. */

            if (mid == higuy)
                mid = loguy;

            /* A[loguy] <= A[mid], A[higuy] > A[mid]; so condition at top
               of loop is re-established */
        }

        /*     A[i] <= A[mid] for lo <= i < loguy,
               A[i] > A[mid] for higuy < i < hi,
               A[hi] >= A[mid]
               higuy < loguy
           implying:
               higuy == loguy-1
               or higuy == hi - 1, loguy == hi + 1, A[hi] == A[mid] */

        /* Find adjacent elements equal to the partition element.  The
           doubled loop is to avoid calling comp(mid,mid), since some
           existing comparison funcs don't work when passed the same value
           for both pointers. */

        higuy += width;
        if (mid < higuy) {
            do  {
                higuy -= width;
            } while (higuy > mid && __COMPARE(context, higuy, mid) == 0);
        }
        if (mid >= higuy) {
            do  {
                higuy -= width;
            } while (higuy > lo && __COMPARE(context, higuy, mid) == 0);
        }

        /* OK, now we have the following:
              higuy < loguy
              lo <= higuy <= hi
              A[i]  <= A[mid] for lo <= i <= higuy
              A[i]  == A[mid] for higuy < i < loguy
              A[i]  >  A[mid] for loguy <= i < hi
              A[hi] >= A[mid] */

        /* We've finished the partition, now we want to sort the subarrays
           [lo, higuy] and [loguy, hi].
           We do the smaller one first to minimize stack usage.
           We only sort arrays of length 2 or more.*/

        if ( higuy - lo >= hi - loguy ) {
            if (lo < higuy) {
                lostk[stkptr] = lo;
                histk[stkptr] = higuy;
                ++stkptr;
            }                           /* save big recursion for later */

            if (loguy < hi) {
                lo = loguy;
                goto recurse;           /* do small recursion */
            }
        }
        else {
            if (loguy < hi) {
                lostk[stkptr] = loguy;
                histk[stkptr] = hi;
                ++stkptr;               /* save big recursion for later */
            }

            if (lo < higuy) {
                hi = higuy;
                goto recurse;           /* do small recursion */
            }
        }
    }

    /* We have sorted the array, except for any pending sorts on the stack.
       Check if there are any, and do them. */

    --stkptr;
    if (stkptr >= 0) {
        lo = lostk[stkptr];
        hi = histk[stkptr];
        goto recurse;           /* pop subarray from stack */
    }
    else
        return;                 /* all subarrays done */
}

///end qsort

// ????????,?????????
__s32  rat_file_list_sort(ScanInfo_t *pScanInfo ,  __s32 total )
{
		__s32 i;
		
		char **pitem_head = NULL;
		ScanFileList_t* first = pScanInfo->FirstList;
		ScanFileList_t *pitem_tmp = NULL;

		ScanFileList_t *pit1,*pit2;	
		
		
		pitem_head = esMEMS_Balloc(sizeof(char *)*total);
		if(NULL == pitem_head)
		{
			__wrn("men not enough...\n");
			//RAT_LEAVE_CRITICAL_SECTION;
			return EPDK_FAIL;
		}
		eLIBs_memset(pitem_head, 0, sizeof(char *)*total);	

		__wrn("count=%d\n", total);

		//*ptmp = *pitem_head;
		pitem_tmp = first;
		for(i=0; i<total; i++)
		{
			
			pitem_head[i] = (char *)pitem_tmp;
			__wrn("ptmp = %08x	pitem_tmp = %08x\n",pitem_head[i],pitem_tmp);
			pitem_tmp = pitem_tmp->Next;
			//*ptmp = *ptmp + 1;
		}
		
		{
			ScanFileList_t *ppp;
			pitem_tmp = first;
			//*ptmp = *pitem_head; 
			for(i=0; i<total; i++)
			{
				__wrn("ptmp = %08x\n",pitem_head[i]);
				ppp = (ScanFileList_t *)pitem_head[i];
				__wrn("name = %s \n",ppp->name);
			}
		}
		
		//esKRNL_TimeDly(1000);
		//__wrn("*pitem_head->name = %s\n",*pitem_head->name);
		
		qsort(pitem_head,total,sizeof(char *),filelist_compare);

		{
			ScanFileList_t *ppp;
			pitem_tmp = first;
			//*ptmp = *pitem_head; 
			for(i=0; i<total; i++)
			{
				__wrn("ptmp = %08x\n",pitem_head[i]);
				ppp = (ScanFileList_t *)pitem_head[i];
				__wrn("name = %s \n",ppp->name);
			}

		}
		
		
		pit1 = (ScanFileList_t *)pitem_head[0];
		pit2 = (ScanFileList_t *)pitem_head[0];
		
		for(i=0; i<total-1; i++)
		{
			pit1 = (ScanFileList_t *)pitem_head[i];
			pit2 = (ScanFileList_t *)pitem_head[i+1];
			pit1->Next = pit2;
			pit2->Prev = pit1;
		}
		
		pit1 = (ScanFileList_t *)pitem_head[0];
		pit1->Prev = pit2;
		pit2->Next = pit1;
		
		pScanInfo->FirstList = pit1;
		pScanInfo->LastList = pit2;

		if(pitem_head)
		{
			esMEMS_Bfree(pitem_head,sizeof(char *)*total);
			pitem_head = NULL;
		}
		
		//first = file_list_sort ( first , count ) ;	//???????????, ???????????
		//list = first->previous ;		 //?????????????,?list ???????????
	}



#endif

/*
****************************************************************************************************
*
*             							ScanFilesBegin
*
*  Description:
*				-	begin the scan process.
*
*  Parameters:
*   pDir		-  	pointer to scan directory.
*   FileTypes	-  	types of scan ebook.
*   OnceCnt		-  	read items number when read once. 
*					if zero, I will read all items one time.
*
*  Return value:
*   NULL   		-  	setup scan process failed.
*	others		-	handle to scan process.
****************************************************************************************************
*/
HSCAN ScanFilesBegin(const char *pDir, __u64 FileTypes, int OnceCnt)
{
	ScanInfo_t 	*pScanInfo;
	ScanFileList_t	*pList;
	HSTACK				 hStack;

	if (NULL == pDir) {
		__wrn("invalid paramter for ScanBookFile\n");
		return NULL;
	}
	pScanInfo = (ScanInfo_t *)rat_malloc(sizeof(ScanInfo_t));
	if (NULL == pScanInfo) {
		__wrn("malloc failed\n");
		return NULL;
	}
	pList = (ScanFileList_t *)rat_malloc(FILELIST_LEN);
	if (NULL == pList) {
		__wrn("malloc failed\n");
		rat_free(pScanInfo);
		return NULL;
	}
	
	/* initialize file list structure */
	pList->Prev 		= NULL;
	pList->Next			= NULL;
	pList->Alloc		= FILELIST_LEN - LIST_DATA_OFFSET(pList);
	pList->Size 		= 0;
	pList->Top			= 0;
	
	/* initialize scan cursor structure */
	hStack = ScanStackAlloc();
	if (NULL == hStack) {
		__wrn("stack allocate failed\n");
		rat_free(pList);
		rat_free(pScanInfo);
		return NULL;
	}
	pScanInfo->FirstList  = pList;
	pScanInfo->LastList	  = pList;
	
	pScanInfo->ScanCursor.CurList = pList;
	pScanInfo->ScanCursor.Offset = 0;
	
	pScanInfo->ScanTypes  = FileTypes;
	pScanInfo->OnceCnt    = OnceCnt;
	pScanInfo->CurCnt     = 0;
	pScanInfo->hStack	  = hStack;
	pScanInfo->pCurDir	  = NULL;
	eLIBs_strcpy(pScanInfo->CurPath, pDir);
	
	pScanInfo->OutEntry.Format 	= RAT_MEDIA_TYPE_UNKNOWN;
	pScanInfo->OutEntry.Path 	= NULL;
	/* setup scan work process */
	if (EPDK_OK != __SetupScanProcess(pScanInfo)) {
		__DestoryScanInfo(pScanInfo);
		__wrn("setup scan process failed\n");
		return NULL;
	}
	return (HSCAN)pScanInfo;
}


/*
****************************************************************************************************
*
*             							ScanMoveToFirst
*
*  Description:
*				-	move scan handle cursor position to first entry.
*				  	
*  Parameters:
*	hScan		-	handle to scan information.
*
*  Return value:
*   EPDK_OK   	-  	move succeeded.
*	EPDK_FAIL	-	move failed.
****************************************************************************************************
*/
int ScanMoveToFirst(HSCAN hScan)
{
	ScanInfo_t *pScanInfo;
	
	if (NULL == hScan) {
		__wrn("invalid parameter\n");
		return EPDK_FAIL;
	}
	pScanInfo = (ScanInfo_t *)hScan;
	
	pScanInfo->ScanCursor.CurList = pScanInfo->FirstList;
	pScanInfo->ScanCursor.Offset = 0;
	
	__msg("scan cursor move to first...\n");

	return EPDK_OK;
}

int __ScanMoveCursorToFirst(HSCAN hScan, ScanListCursor_t* cursor)
{
	ScanInfo_t *pScanInfo;
	
	if (NULL == hScan) {
		__wrn("invalid parameter\n");
		return EPDK_FAIL;
	}
	pScanInfo = (ScanInfo_t *)hScan;
	
	cursor->CurList = pScanInfo->FirstList;
	cursor->Offset = 0;
	
	return EPDK_OK;
}

/*
****************************************************************************************************
*
*             							ScanMoveToNext
*
*  Description:
*				-	move scan handle cursor position to next entry.
*				  	
*  Parameters:
*	hScan		-	handle to scan information.
*
*  Return value:
*   EPDK_OK   	-  	move succeeded.
*	EPDK_FAIL	-	move failed.
****************************************************************************************************
*/
int ScanMoveToNext(HSCAN hScan)
{
	ScanInfo_t 		*pScanInfo;
	ScanFileEntry_t	*pEntry;
	
	if (NULL == hScan) {
		__wrn("invalid parameter\n");
		return EPDK_FAIL;
	}
	pScanInfo = (ScanInfo_t *)hScan;
#if (SCAN_SORT == 0) //add by hot.lee	
	if ((pScanInfo->ScanCursor.Offset == pScanInfo->ScanCursor.CurList->Top))
	{
		if (NULL == pScanInfo->ScanCursor.CurList->Next) 
		{
			int Count;
			
			/* the next entry have not read out, should read it by self */
			Count = ScanFilesOneTime(pScanInfo);
			
			if (0 == Count) 
			{
				/* have no valid ebook files, scan is done */
				pScanInfo->ScanCursor.Offset = pScanInfo->ScanCursor.CurList->Size;
				return EPDK_FAIL;
			}
			if (pScanInfo->ScanCursor.Offset == pScanInfo->ScanCursor.CurList->Top) 
			{
				/* the read out entry not located in current file list */
				pScanInfo->ScanCursor.CurList = pScanInfo->ScanCursor.CurList->Next;
				pScanInfo->ScanCursor.Offset = 0;
				return EPDK_OK;
			}
		}
		else 
		{
			pScanInfo->ScanCursor.CurList = pScanInfo->ScanCursor.CurList->Next;
			pScanInfo->ScanCursor.Offset = 0;
			return EPDK_OK;
		}
	}
	pEntry = (ScanFileEntry_t *)(pScanInfo->ScanCursor.CurList->Data + \
									  pScanInfo->ScanCursor.Offset);
	pScanInfo->ScanCursor.Offset += pEntry->RecLen;
#else
	if (NULL == pScanInfo->ScanCursor.CurList->Next) 
	{
		int Count;
		
		/* the next entry have not read out, should read it by self */
		Count = ScanFilesOneTime(pScanInfo);
		
		if (0 == Count) 
		{
			/* have no valid ebook files, scan is done */
			pScanInfo->ScanCursor.Offset = pScanInfo->ScanCursor.CurList->Size;
			return EPDK_FAIL;
		}
		//if (pScanInfo->ScanCursor.Offset == pScanInfo->ScanCursor.CurList->Top) 
		{
			/* the read out entry not located in current file list */
			pScanInfo->ScanCursor.CurList = pScanInfo->ScanCursor.CurList->Next;
			pScanInfo->ScanCursor.Offset = 0;
			return EPDK_OK;
		}
	}
	else 
	{
		pScanInfo->ScanCursor.CurList = pScanInfo->ScanCursor.CurList->Next;
		pScanInfo->ScanCursor.Offset = 0;
		return EPDK_OK;
	}

#endif
	return EPDK_OK;
}

int __ScanMoveCursorToNext(HSCAN hScan, ScanListCursor_t* cursor)
{
	ScanFileEntry_t	*pEntry;
#if (SCAN_SORT == 0) //add by hot.lee		
	if ((cursor->Offset == cursor->CurList->Top)) 
	{
		if (NULL == cursor->CurList->Next) 
		{			
			return EPDK_FAIL;
		}
		else
		{
			cursor->CurList = cursor->CurList->Next;
			cursor->Offset = 0;
			return EPDK_OK;
		}
	} 	
	
	pEntry = (ScanFileEntry_t *)(cursor->CurList->Data + cursor->Offset);
	cursor->Offset += pEntry->RecLen;
#else
	if (NULL == cursor->CurList->Next) 
	{			
		return EPDK_FAIL;
	}
	else
	{
		cursor->CurList = cursor->CurList->Next;
		cursor->Offset = 0;
		return EPDK_OK;
	}

#endif	
	return EPDK_OK;
}

/*
****************************************************************************************************
*
*             							ScanMoveToPrev
*
*  Description:
*				-	move scan handle cursor position to prev entry.
*				  	
*  Parameters:
*	hScan		-	handle to scan information.
*
*  Return value:
*   EPDK_OK   	-  	move succeeded.
*	EPDK_FAIL	-	move failed.
****************************************************************************************************
*/
int ScanMoveToPrev(HSCAN hScan)
{
	ScanInfo_t 		*pScanInfo;
	ScanFileEntry_t	*pEntry;
	
	if (NULL == hScan) {
		__wrn("invalid parameter\n");
		return EPDK_FAIL;
	}
	pScanInfo = (ScanInfo_t *)hScan;
#if (SCAN_SORT == 0) //add by hot.lee		
	if (0 == (pScanInfo->ScanCursor.Offset)) 
	{
		if (NULL == (pScanInfo->ScanCursor.CurList->Prev))
		{
			/* have reached begin of file list */
			return EPDK_FAIL;
		}
		else 
		{
			/* move to the last entry of current prev list */
			pScanInfo->ScanCursor.CurList = pScanInfo->ScanCursor.CurList->Prev;
			pScanInfo->ScanCursor.Offset = pScanInfo->ScanCursor.CurList->Top;
			return EPDK_OK;
		}
	}

	if ((pScanInfo->ScanCursor.CurList->Size) == (pScanInfo->ScanCursor.Offset)) {
		pScanInfo->ScanCursor.Offset = pScanInfo->ScanCursor.CurList->Top;
		return EPDK_OK;
	}
	
	pEntry = (ScanFileEntry_t *)(pScanInfo->ScanCursor.CurList->Data + \
									  pScanInfo->ScanCursor.Offset);
	pScanInfo->ScanCursor.Offset = pEntry->Prev;
#else
	if (NULL == (pScanInfo->ScanCursor.CurList->Prev))
	{
		/* have reached begin of file list */
		return EPDK_FAIL;
	}
	else 
	{
		/* move to the last entry of current prev list */
		pScanInfo->ScanCursor.CurList = pScanInfo->ScanCursor.CurList->Prev;
		pScanInfo->ScanCursor.Offset = pScanInfo->ScanCursor.CurList->Top;
		return EPDK_OK;
	}

#endif
	return EPDK_OK;
}

int __ScanMoveCurosrToPrev(HSCAN hScan, ScanListCursor_t* cursor)
{
	ScanFileEntry_t	*pEntry;

#if (SCAN_SORT == 0) //add by hot.lee		
	if (0 == (cursor->Offset)) 
	{
		if (NULL == (cursor->CurList->Prev)) 
		{
			/* have reached begin of file list */
			return EPDK_FAIL;
		}
		else 
		{
			/* move to the last entry of current prev list */
			cursor->CurList = cursor->CurList->Prev;
			cursor->Offset = cursor->CurList->Top;
			return EPDK_OK;
		}
	}

	if ((cursor->CurList->Size) == (cursor->Offset)) 
	{
		cursor->Offset = cursor->CurList->Top;
		return EPDK_OK;
	}
	
	pEntry = (ScanFileEntry_t *)(cursor->CurList->Data + \
		cursor->Offset);
	cursor->Offset = pEntry->Prev;
#else
	if (NULL == (cursor->CurList->Prev)) 
	{
		/* have reached begin of file list */
		return EPDK_FAIL;
	}
	else 
	{
		/* move to the last entry of current prev list */
		cursor->CurList = cursor->CurList->Prev;
		cursor->Offset = cursor->CurList->Top;
		return EPDK_OK;
	}

#endif
	return EPDK_OK;
}

/*
****************************************************************************************************
*
*             							CursorIsDone
*
*  Description:
*				-	check scan handle cursor position pointing at the end entry.
*				  	
*  Parameters:
*	hScan		-	handle to scan information.
*
*  Return value:
*   EPDK_TRUE   -  	cursor position pointing the end entry.
*	EPDK_FALSE	-	cursor position not pointing at the end entry.
****************************************************************************************************
*/
__bool ScanIsDone(HSCAN hScan)
{
	ScanInfo_t *pScanInfo;
	
	if (NULL == hScan) {
		__wrn("invalid parameter\n");
		return EPDK_FALSE;
	}
	pScanInfo = (ScanInfo_t *)hScan;
#if (SCAN_SORT == 0) //add by hot.lee		
	if ((EPDK_TRUE == ScanStackEmpty(pScanInfo->hStack)) && \
		(NULL == pScanInfo->pCurDir)) {
		if ((pScanInfo->ScanCursor.CurList == pScanInfo->LastList) && \
			(pScanInfo->ScanCursor.Offset == pScanInfo->ScanCursor.CurList->Size)) {
			return EPDK_TRUE;
		}
	}
#else
	if ((EPDK_TRUE == ScanStackEmpty(pScanInfo->hStack)) && \
		(NULL == pScanInfo->pCurDir)) {
		if ((pScanInfo->ScanCursor.CurList == pScanInfo->LastList)) {
			return EPDK_TRUE;
		}
	}

#endif
	return EPDK_FALSE;
}

/*
****************************************************************************************************
*
*             							ScanFree
*
*  Description:
*				-	free scan handle.
*
*  Parameters:
*	hScan		-	handle to scan information.
*
*  Return value:
*   	   		NONE
****************************************************************************************************
*/
void ScanFree(HSCAN hScan)
{
	__DestoryScanInfo((ScanInfo_t *)hScan);
}


/*
****************************************************************************************************
*
*             							ScanGetCurrent
*
*  Description:
*				-	get current scaned file entry.
*				  	
*  Parameters:
*	hScan		-	handle to scan information.
*
*  Return value:
*   NULL   		-  	get current entry failed.
*	others		-	pointer to scan cursor current entry.
****************************************************************************************************
*/
ScanedEntry_t *ScanGetCurrent(HSCAN hScan)
{
	ScanInfo_t 		*pScanInfo;
	ScanFileEntry_t	*pEntry;
	
	if (NULL == hScan) {
		__wrn("invalid parameter\n");
		return NULL;
	}
	pScanInfo = (ScanInfo_t *)hScan;
#if (SCAN_SORT == 0) //add by hot.lee		
	if ((pScanInfo->ScanCursor.Offset == pScanInfo->ScanCursor.CurList->Size)) {
		if (NULL == pScanInfo->ScanCursor.CurList->Next) {
			
			/* buffer have no data */
			return NULL;
		} else {
			pScanInfo->ScanCursor.CurList = pScanInfo->ScanCursor.CurList->Next;
			pScanInfo->ScanCursor.Offset = 0;
		}
	}
	pEntry = (ScanFileEntry_t *)(pScanInfo->ScanCursor.CurList->Data + \
									  pScanInfo->ScanCursor.Offset);
	pScanInfo->OutEntry.Format = pEntry->Format;
	pScanInfo->OutEntry.Path   = (char *)(pEntry->Name);
#else
	pEntry = (ScanFileEntry_t *)(pScanInfo->ScanCursor.CurList->Data);
	pScanInfo->OutEntry.Format = pEntry->Format;
	pScanInfo->OutEntry.Path   = (char *)(pEntry->Name);

#endif
	return &(pScanInfo->OutEntry);
}

ScanedEntry_t *__ScanGetCurrentCursorData(HSCAN hScan, ScanListCursor_t* cursor)
{
	ScanInfo_t 		*pScanInfo;
	ScanFileEntry_t	*pEntry;
	
	if (NULL == hScan) {
		__wrn("invalid parameter\n");
		return NULL;
	}
	pScanInfo = (ScanInfo_t *)hScan;
#if (SCAN_SORT == 0) //add by hot.lee		
	if ((cursor->Offset == cursor->CurList->Size)) {
		if (NULL == cursor->CurList->Next) {
			
			/* buffer have no data */
			return NULL;
		} else {
			cursor->CurList = cursor->CurList->Next;
			cursor->Offset = 0;
		}
	}
	pEntry = (ScanFileEntry_t *)(cursor->CurList->Data + \
		cursor->Offset);
	pScanInfo->OutEntryCursor.Format = pEntry->Format;
	pScanInfo->OutEntryCursor.Path   = (char *)(pEntry->Name);
#else
	__wrn("...\n");

	if (NULL == cursor->CurList) 
	{
		__wrn("buffer have no data \n");
		//esKRNL_TimeDly(1000);
		return NULL;
	} 
	
	__wrn("...\n");
			
	pEntry = (ScanFileEntry_t *)(cursor->CurList->Data);
	__wrn("pEntry->Name = %s \n",pEntry->Name);
	pScanInfo->OutEntryCursor.Format = pEntry->Format;
	pScanInfo->OutEntryCursor.Path	 = (char *)(pEntry->Name);

	__wrn("...\n");

#endif
	return &(pScanInfo->OutEntryCursor);
}

/*
****************************************************************************************************
*
*             							ScanGetCurCnt
*
*  Description:
*				-	get current scaned file count.
*				  	
*  Parameters:
*	hScan		-	handle to scan information.
*
*  Return value:
*   The number of current files number, -1 if failed.
****************************************************************************************************
*/
int ScanGetCurCnt(HSCAN hScan)
{
	ScanInfo_t *pScanInfo;
	
	if (NULL == hScan) {
		__wrn("invalid parameter\n");
		return -1;
	}
	pScanInfo = (ScanInfo_t *)hScan;
	
	return pScanInfo->CurCnt;
}


static int __SetupScanProcess(ScanInfo_t *pScanInfo)
{
	int Count;
    char* p;
    char* pblash;
    char str_root[RAT_MAX_FULL_PATH_LEN];
	
	/* push current path to stack, ready for scan files */	
    p = pScanInfo->CurPath;
    
    do{
        if(NULL == p)
        {
            break;
        }
        pblash = eLIBs_strchr(p, '/');
        if(NULL == pblash)
        {
            if(NULL != p && 0 != *p)
            {
                eLIBs_strcpy(str_root, p);
                ScanStackPush(pScanInfo->hStack, str_root);            
                __wrn("search root=%s\n", str_root);
            }
            break;
        }
        else
        {
            eLIBs_strncpy(str_root, p, pblash-p);
            str_root[pblash-p] = 0;
            ScanStackPush(pScanInfo->hStack, str_root);  
            __wrn("search root=%s\n", str_root);
            p = pblash+1;
        }
	}while(1);
	
	if (0 == pScanInfo->OnceCnt) {
		
		/* scan all files at one time */
		Count = __ScanAllFiles(pScanInfo);
		pScanInfo->CurCnt = Count;
		if (0 == Count) {
			__wrn("have not find any valid media file\n");
			
			
		
		//	return EPDK_FAIL;
		}
		
		return EPDK_OK;
	} else {
		/* scan pScanInfo->OnceCnt files one time */
		Count = ScanFilesOneTime(pScanInfo);
		
		if (0 == Count) {
			__wrn("have not find any valid ebook file\n");
		//	return EPDK_FAIL;
		}
		
		return EPDK_OK;
	}
}


static int __ScanAllFiles(ScanInfo_t *pScanInfo)
{
	ES_DIRENT	*pDirent;
	int			 Format = RAT_MEDIA_TYPE_UNKNOWN;
	int			 Count  = 0;

    do{//¡ä|¨¤¨ª¨º?2?¨¤¨¤D¨ª¨¦¡§?¨¨
        __s32 ret;
        __s32 cnt;
        __s32 val;
        __s32 num;
        __s32 i;
		__s32 fsize;
        ES_FILE* fp;
        char str_db_name[32]={0};
        char str_db_path[1024]={0};
        char str_file_item[1024]={0};

        cnt = 0;
        fp = NULL;
        
        if(PHOTO_SCAN_FAVOR == pScanInfo->ScanTypes)
        {
            eLIBs_strcpy(str_db_name, "photo.fdb");
        }
        else if(MUSIC_SCAN_FAVOR == pScanInfo->ScanTypes)
        {
            eLIBs_strcpy(str_db_name, "music.fdb");
        }
        else if(MOVIE_SCAN_FAVOR == pScanInfo->ScanTypes)
        {
            eLIBs_strcpy(str_db_name, "movie.fdb");
        }
        else if(EBOOK_SCAN_FAVOR == pScanInfo->ScanTypes)
        {
            eLIBs_strcpy(str_db_name, "ebook.fdb");
        }
        else
        {
            break;
        }

        eLIBs_sprintf(str_db_path, "%c:\\%s%s", pScanInfo->CurPath[0],
            "favor_db\\", str_db_name);

		DebugPrintf("str_db_path=%s\n", str_db_path);

        fp = eLIBs_fopen(str_db_path, "rb");
        if(NULL == fp)
        {
            __wrn("file does not exist,str_db_path=%s\n", str_db_path);
            goto end;
        }

        ret = eLIBs_fread(&val, 1, 4, fp);
        if(4 != ret)
        {
            __wrn("eLIBs_fread fail...\n");
            goto end;
        }

		fsize = val;
        DebugPrintf("fsize=%d\n", fsize);

        eLIBs_fseek(fp, 4, SEEK_SET);
        ret = eLIBs_fread(&val, 1, 4, fp);
        if(4 != ret)
        {
            DebugPrintf("eLIBs_fread fail...\n");
            
            goto end;
        }

        num = val;

		DebugPrintf("num=%d\n", num);

        eLIBs_fseek(fp, 0, SEEK_END);
        ret = eLIBs_ftell(fp);
        if(ret < num*1024+16
            ||ret < fsize)
        {
            goto end;
        }


        eLIBs_fseek(fp, 16, SEEK_SET);
        

		DebugPrintf("num=%d\n", num);
		
        for(i = 0 ; i < num ; i++)
        {
            ret = eLIBs_fread(str_file_item, 1, 1024, fp);
            if(1024 != ret)
            {
                __wrn("eLIBs_fread fail...\n");
                goto end;
            }

            str_file_item[0] = str_db_path[0];//DT???¡¤???¨¬¡¤?

			DebugPrintf("str_file_item=%s\n", str_file_item);

            if (EPDK_OK != __InsertFileEntryToList(str_file_item,
														   0,
														   pScanInfo))
            {
                __wrn("__InsertFileEntryToList fail...\n");
                goto end;
            }
        }
        

        cnt = num;
        
        end:;
        if(fp)
        {
            eLIBs_fclose(fp);
            fp = NULL;
        }

		DebugPrintf("cnt=%d\n", cnt);
		
        return cnt;
        
    }while(0);
    
    while(EPDK_TRUE != ScanStackEmpty(pScanInfo->hStack)) {
    	
    	/* pop directory from stack and save to pScanInfo->CurPath */
    	eLIBs_strcpy(pScanInfo->CurPath, ScanStackPop(pScanInfo->hStack));
    	pScanInfo->pCurDir = eLIBs_opendir(pScanInfo->CurPath);
        if (NULL == pScanInfo->pCurDir) {
        	return Count;
        }
        
        while (NULL != (pDirent = eLIBs_readdir(pScanInfo->pCurDir))) {
        	
        	if (pDirent->fatdirattr & FSYS_ATTR_DIRECTORY) {
			
				/* ignore "." and ".." sub-directory */
        		if (pDirent->d_name[0] == '.') {
        			continue;
        		}
        		/* sub-directory push to stack */
				eLIBs_strcpy(pScanInfo->tempPath, pScanInfo->CurPath);
				eLIBs_strcat(pScanInfo->tempPath, "\\");
				eLIBs_strcat(pScanInfo->tempPath, (char*)pDirent->d_name);
				ScanStackPush(pScanInfo->hStack, pScanInfo->tempPath);
			} else {
				if (EPDK_TRUE == __ValidFileType((const char *)(pDirent->d_name),
												  pScanInfo->ScanTypes,
												  &Format)) {
					/* store valid ebook file full path to pScanInfo file list */
					eLIBs_strcpy(pScanInfo->tempPath, pScanInfo->CurPath);
					eLIBs_strcat(pScanInfo->tempPath, "\\");
					eLIBs_strcat(pScanInfo->tempPath, (char*)pDirent->d_name);
					if (EPDK_OK != __InsertFileEntryToList(pScanInfo->tempPath,
														   Format,
														   pScanInfo)) {
						__wrn("insert file entry to list failed\n");
						eLIBs_closedir(pScanInfo->pCurDir);
						pScanInfo->pCurDir = NULL;
						return Count;
					}
					Count++;
					pScanInfo->CurCnt++;//add by hot.lee  __InsertFileEntryToList() will use this member
                    if(g_rat_search_cb)
                    {
                        esKRNL_CallBack(g_rat_search_cb, g_rat_search_arg);
                    }
				}
			}
        }
        /* close current open directory, continue to scan sub-directory */
        eLIBs_closedir(pScanInfo->pCurDir);
        pScanInfo->pCurDir = NULL;
    }
    return Count;
}

int ScanFilesOneTime(ScanInfo_t *pScanInfo)
{
	ES_DIRENT *pDirent;
	int		   Format = RAT_MEDIA_TYPE_UNKNOWN;
	int		   Count  = 0;
    
    /* should open directory if necessary */
    if (NULL == pScanInfo->pCurDir) {
    	if (EPDK_TRUE == ScanStackEmpty(pScanInfo->hStack)) {
			pScanInfo->CurCnt += Count;
        	return Count;
        }
    	/* pop directory from stack and save to pScanInfo->CurPath */
    	eLIBs_strcpy(pScanInfo->CurPath, ScanStackPop(pScanInfo->hStack));
    	pScanInfo->pCurDir = eLIBs_opendir(pScanInfo->CurPath);
        if (NULL == pScanInfo->pCurDir) {
			pScanInfo->CurCnt += Count;
        	return Count;
        }
    }
    while(1) {
        while (NULL != (pDirent = eLIBs_readdir(pScanInfo->pCurDir))) {
        	
        	if (pDirent->fatdirattr & FSYS_ATTR_DIRECTORY) {
			
				/* ignore "." and ".." sub-directory */
        		if (pDirent->d_name[0] == '.') {
        			continue;
        		}
        		/* sub-directory push to stack */
				rat_sprintf(pScanInfo->tempPath, "%s\\%s", 
							 pScanInfo->CurPath, pDirent->d_name);
				ScanStackPush(pScanInfo->hStack, pScanInfo->tempPath);
			} else {
				if (EPDK_TRUE == __ValidFileType((const char *)(pDirent->d_name), \
												  pScanInfo->ScanTypes,
												  &Format)) {
					/* store valid ebook file full path to pScanInfo file list */
					rat_sprintf(pScanInfo->tempPath, "%s\\%s", 
								 pScanInfo->CurPath, pDirent->d_name);
					if (EPDK_OK != __InsertFileEntryToList(pScanInfo->tempPath,
														   Format,
														   pScanInfo)) {
						__wrn("insert file entry to list failed\n");
						eLIBs_closedir(pScanInfo->pCurDir);
						pScanInfo->pCurDir = NULL;
						//pScanInfo->CurCnt += Count;//detele by hot.lee
						return Count;
					}
					/* get one valid ebook file */
					Count++;
					pScanInfo->CurCnt++; //add by hot.lee ,__InsertFileEntryToList() will use it
                    if(g_rat_search_cb)
                    {
                        esKRNL_CallBack(g_rat_search_cb, g_rat_search_arg);
                    }
				}
			}
			if (Count >= pScanInfo->OnceCnt) {
				/* read once work finished */
				//pScanInfo->CurCnt += Count;//detele by hot.lee
				return Count;
			}
        }
        /* close current open directory, continue to scan sub-directory */
        eLIBs_closedir(pScanInfo->pCurDir);
        pScanInfo->pCurDir = NULL;
        
        /* check sub-directory stack empty or not */
        if (EPDK_TRUE == ScanStackEmpty(pScanInfo->hStack)) {
        	break;
        }
        
        /* pop directory from stack and save to pScanInfo->CurPath */
    	eLIBs_strcpy(pScanInfo->CurPath, ScanStackPop(pScanInfo->hStack));
    	pScanInfo->pCurDir = eLIBs_opendir(pScanInfo->CurPath);
        if (NULL == pScanInfo->pCurDir) {
			//pScanInfo->CurCnt += Count;//detele by hot.lee
        	return Count;
        }
    }
    
    /* not enough valid ebook file for read */
	//pScanInfo->CurCnt += Count; //detele by hot.lee
    return Count;
}

static int __InsertFileEntryToList( char *filePath, 
								   int format,
								   ScanInfo_t *pScanInfo)
{
#if (SCAN_SORT == 0)
	ScanFileList_t  *pLastList;
	ScanFileEntry_t *pEntry;
	__u32				  RecLen;
	
	/* insert file full path to the last file list of pScanInfo */
	pLastList = pScanInfo->LastList;
	
	RecLen = RAT_ALIGN(ENTRY_NAME_OFFSET(pEntry) + eLIBs_strlen(filePath) + 1, sizeof(__u64));
	
	/* check the last file list have enough space for filePath insert */
	if (RecLen > (pLastList->Alloc - pLastList->Size)) {
		
		/* free space not enough, allocate a new file list structure */
		ScanFileList_t  *pNewList;
		pNewList = (ScanFileList_t *)rat_malloc(FILELIST_LEN);
		if (NULL == pNewList) {
			__wrn("malloc failed\n");
			return EPDK_FAIL;
		}
		/* initialize allocated file list */
		pNewList->Prev 		= pLastList;
		pLastList->Next 	= pNewList;
		pNewList->Next		= NULL;
		pNewList->Alloc		= FILELIST_LEN - LIST_DATA_OFFSET(pNewList);
		pNewList->Size 		= 0;
		pNewList->Top 		= 0;	
		
		/* insert filePath to the allocated file list */
		pScanInfo->LastList = pNewList;
		pLastList 			= pScanInfo->LastList;
	}
	
	/* store filePath to the end of last file list */
	pEntry 			= (ScanFileEntry_t	*)(pLastList->Data + pLastList->Size);
	pEntry->RecLen	= RecLen;
	pEntry->Prev 	= pLastList->Top;	/* prev entry location */
	pEntry->Format	= format;
	eLIBs_strcpy((char *)(pEntry->Name), filePath);
	
	/* adjust the last file list management information */
	pLastList->Top	  = pLastList->Size;
	pLastList->Size  += RecLen;
	
#else //add by hot.lee
	ScanFileList_t	*pNewList;
	ScanFileList_t  *pLastList;

	ScanFileEntry_t *pEntry,*pTmpEntry;
	__s32 i;
	
	__u32				  RecLen;
	char Path[1024];
	char *p;
	__s32 ret  = -2;

	pLastList = pScanInfo->LastList;

	RecLen = RAT_ALIGN(8+LIST_DATA_OFFSET(pLastList) + ENTRY_NAME_OFFSET(pEntry) + eLIBs_strlen(filePath) + 1, sizeof(__u64));
	__wrn("pScanInfo->CurCnt = %d...\n",pScanInfo->CurCnt);

	/* check the last file list have enough space for filePath insert */
	//if (RecLen > (pLastList->Alloc - pLastList->Size)) 
	if(pScanInfo->CurCnt > 0)
	{		
		/* allocate a new file list structure */

		//????ScanFileList_t+filePath,???ScanFileList_t??ScanFileEntry_t,ScanFileEntry_t?????
		////////////////
		//ScanFileList_t//
		//----------//
		//ScanFileList_t//
		//-----------//
		//filePath        //
		//////////////
		__wrn("RecLen = %d ...\n",RecLen);
		pNewList = (ScanFileList_t *)rat_malloc(RecLen);
		if (NULL == pNewList) {
			__wrn("malloc failed\n");
			return EPDK_FAIL;
		}
		/* initialize allocated file list */
		pNewList->Prev		= pNewList;
		pNewList->Next		= pNewList;
		
		/* insert filePath to the allocated file list */
		//__wrn("filePath = %s...\n",filePath);
		
		/* store filePath to the end of last file list */
		pEntry			= (ScanFileEntry_t	*)(pNewList->Data);	
		pEntry->Format	= format;
		//esKRNL_TimeDly(1000);
		eLIBs_strcpy((char *)(pEntry->Name), filePath);
		__wrn("pScanInfo->CurCnt = %d...\n",pScanInfo->CurCnt);
		
		p = eLIBs_strchrlast(filePath,'\\')+1;
		eLIBs_encoding_stream_convert(EPDK_CHARSET_ENM_UTF8,EPDK_CHARSET_ENM_GBK,p,3,pEntry->uin_name,3);//???3??????
	
		//pMoveList = pScanInfo->FirstList;
		__ScanMoveCursorToFirst((HSCAN)pScanInfo, &pScanInfo->TmpCursor);
		//?????ScanFileList_t?????????
#if 0  //?????		
		for(i=0; i<pScanInfo->CurCnt; i++)//?????????
		{		
		    __s32 type1,type2;
			char  tmpPath[1024];
			ScanedEntry_t *scn_ret;

			__wrn("...\n");
			
			scn_ret = __ScanGetCurrentCursorData((HSCAN)pScanInfo , &pScanInfo->TmpCursor);
			if(NULL == scn_ret)
			{
			    __err("scan list err.....\n");
				break;
			}
			
			eLIBs_strcpy(tmpPath,pScanInfo->OutEntryCursor.Path);
			//__wrn("tmpPath = %s...\n",tmpPath);
			p = eLIBs_strchrlast(tmpPath,'\\') + 1;
			eLIBs_strcpy(Path, p);	
			__wrn("...\n");

			p = eLIBs_strchrlast(filePath,'\\') + 1;

			//__msg(" ret = %d\n " , ret );
			type1 = rat_file_item_name_class(Path);//path??(p)filePath,continue
			type2 = rat_file_item_name_class(p);	
			__wrn("type1 = %d type2 = %d\n",type1,type2);
			/*if(type1 != type2 && (type1 != IS_CHN))
			{
				ret = eLIBs_strcmp( Path , p ) ;	//?????
			}
			else*/
			{
				if(IS_NUM == type1 && IS_NUM == type2)
				{
					ret = rat_num_name_cmp( Path , p );
					__wrn("ret = %d \n",ret);
				}
				else if((type1 == IS_NUM || type1 == IS_LETTER) && (type2 == IS_CHN))
				{
					ret = -1;//<
				}
				else if(type1 == IS_NUM && (type2 ==IS_LETTER || type2 == IS_CHN))
				{
					ret = -1;//<
				}
				else if((type1 == IS_LETTER || type2 == IS_CHN) && type2 == IS_NUM)
				{
					ret = 1;//>
				}				
				else
				{
					ret = rat_chn_name_cmp(Path,p);
				}
			}	

			if(ret >= 0)//path > p(filePath)
			{
				break; //jump out for()
			}

			__wrn("...\n");
			if (EPDK_OK != __ScanMoveCursorToNext((HSCAN)pScanInfo, &pScanInfo->TmpCursor))
			{
				i++;
				break;
			}
			__wrn("...\n");

			
		}
#endif
		__wrn("i = %d \n",i);
		///if((i == pScanInfo->CurCnt))//???????
		{
			pNewList->Prev		= pLastList;
			pLastList->Next 	= pNewList;
			pNewList->Next		= pScanInfo->FirstList;//????????
			pScanInfo->FirstList->Prev = pNewList;
			__wrn("...\n");

			/* insert filePath to the allocated file list */
			pScanInfo->LastList = pNewList;
			pLastList 			= pScanInfo->LastList;
			__wrn("...\n");

		}
#if 0		
		else if(0 == i) //??????
		{
		    ScanFileList_t	*pFirstList;
			pFirstList = pScanInfo->FirstList;
			__wrn("...\n");

			pNewList->Prev		= pLastList;
			pLastList->Next 	= pNewList;
			pNewList->Next		= pFirstList;//????????			
			pFirstList->Prev    = pNewList;
			__wrn("...\n");

			pScanInfo->FirstList = pNewList;

			//pNewList = 
		}
		else //??
		{
			ScanFileList_t	*pCurList;
			__wrn("...\n");

			pCurList = pScanInfo->TmpCursor.CurList;

			pNewList->Prev = pCurList->Prev;
			pCurList->Prev->Next = pNewList;
			pNewList->Next = pCurList;
			pCurList->Prev = pNewList;
			__wrn("...\n");
		}
#endif
	}
	else
	{
		/* store filePath to the end of last file list */
		__wrn("...\n");
		pEntry			= (ScanFileEntry_t	*)(pScanInfo->FirstList->Data);	
		pEntry->Format	= format;
		eLIBs_strcpy((char *)(pEntry->Name), filePath);		
	}

	__here__;


	//pScanInfo->LastList->Next = pScanInfo->FirstList;//add by hot.lee for sort
	//pScanInfo->FirstList->Prev = pScanInfo->LastList;

#endif
	return EPDK_OK;
}

static FmtIdx_t ValidFmtTbl[] = {
	{ ".txt",	RAT_MEDIA_TYPE_EBOOK,		EBOOK_SCAN_TXT	},		
	//{ ".h",	    RAT_MEDIA_TYPE_EBOOK,		EBOOK_SCAN_H    },
	//{ ".c",	    RAT_MEDIA_TYPE_EBOOK,		EBOOK_SCAN_C    },
	//{ ".ini",	RAT_MEDIA_TYPE_EBOOK,		EBOOK_SCAN_INI	},
	{ ".lrc",	RAT_MEDIA_TYPE_EBOOK,		EBOOK_SCAN_LRC	},
	{ ".srt",	RAT_MEDIA_TYPE_EBOOK,		EBOOK_SCAN_SRT  },
    { ".ssa",  	RAT_MEDIA_TYPE_EBOOK,      	EBOOK_SCAN_SSA	},
    { ".ass",   RAT_MEDIA_TYPE_EBOOK,     	EBOOK_SCAN_ASS	},
    //{ ".inf",   RAT_MEDIA_TYPE_EBOOK,     	EBOOK_SCAN_INF	},

   	{ ".AVI",  	RAT_MEDIA_TYPE_VIDEO,       MOVIE_SCAN_AVI  },
    { ".DIVX", 	RAT_MEDIA_TYPE_VIDEO,      	MOVIE_SCAN_AVI  },
    { ".PMP",  	RAT_MEDIA_TYPE_VIDEO,      	MOVIE_SCAN_PMP  },
    { ".3GP",  	RAT_MEDIA_TYPE_VIDEO,      	MOVIE_SCAN_3GP  },
    { ".MP4",  	RAT_MEDIA_TYPE_VIDEO,      	MOVIE_SCAN_MP4  },
    { ".M4V",  	RAT_MEDIA_TYPE_VIDEO,     	MOVIE_SCAN_MP4  },
    { ".F4V",  	RAT_MEDIA_TYPE_VIDEO,      	MOVIE_SCAN_FLV  },
    { ".MPG",  	RAT_MEDIA_TYPE_VIDEO,     	MOVIE_SCAN_MPG  },
    { ".MPEG", 	RAT_MEDIA_TYPE_VIDEO,     	MOVIE_SCAN_MPG  },
    { ".VOB",  	RAT_MEDIA_TYPE_VIDEO,     	MOVIE_SCAN_VOB  },
    { ".DAT",  	RAT_MEDIA_TYPE_VIDEO,     	MOVIE_SCAN_DAT  },
    { ".RM",   	RAT_MEDIA_TYPE_VIDEO,     	MOVIE_SCAN_RM   },
    { ".RAM",  	RAT_MEDIA_TYPE_VIDEO,      	MOVIE_SCAN_RM   },
    { ".RMVB",	RAT_MEDIA_TYPE_VIDEO,    	MOVIE_SCAN_RMVB },
    { ".FLV" , 	RAT_MEDIA_TYPE_VIDEO,     	MOVIE_SCAN_FLV  },
    { ".WMV" , 	RAT_MEDIA_TYPE_VIDEO,     	MOVIE_SCAN_WMV  },
    { ".ASF" , 	RAT_MEDIA_TYPE_VIDEO,     	MOVIE_SCAN_ASF  },
    { ".MKV" , 	RAT_MEDIA_TYPE_VIDEO,     	MOVIE_SCAN_MKV  },
    { ".PSR" , 	RAT_MEDIA_TYPE_VIDEO,     	MOVIE_SCAN_PSR  },
    { ".TP"  , 	RAT_MEDIA_TYPE_VIDEO,     	MOVIE_SCAN_TP   },
    { ".TRP" , 	RAT_MEDIA_TYPE_VIDEO,      	MOVIE_SCAN_TS   },
    { ".TS"  , 	RAT_MEDIA_TYPE_VIDEO,      	MOVIE_SCAN_TS   },
    { ".M2TS", 	RAT_MEDIA_TYPE_VIDEO,      	MOVIE_SCAN_TS   },
    { ".MOV", 	RAT_MEDIA_TYPE_VIDEO,      	MOVIE_SCAN_MOV  },

	{ ".MP3",  	RAT_MEDIA_TYPE_AUDIO,     	MUSIC_SCAN_MP3  },
    { ".MP2",  	RAT_MEDIA_TYPE_AUDIO,     	MUSIC_SCAN_MP2  },
    { ".MP1",   RAT_MEDIA_TYPE_AUDIO,     	MUSIC_SCAN_MP1   },
    { ".WMA",  	RAT_MEDIA_TYPE_AUDIO,      	MUSIC_SCAN_WMA   },
    { ".OGG",	RAT_MEDIA_TYPE_AUDIO,    	MUSIC_SCAN_OGG },
    { ".APE" , 	RAT_MEDIA_TYPE_AUDIO,     	MUSIC_SCAN_APE  },
    { ".FLAC" , RAT_MEDIA_TYPE_AUDIO,     	MUSIC_SCAN_FLAC  },
    { ".AC3" , 	RAT_MEDIA_TYPE_AUDIO,     	MUSIC_SCAN_AC3  },
    { ".DTS" , 	RAT_MEDIA_TYPE_AUDIO,     	MUSIC_SCAN_DTS  },
    { ".RA" , 	RAT_MEDIA_TYPE_AUDIO,     	MUSIC_SCAN_RA  },
    { ".AAC"  , RAT_MEDIA_TYPE_AUDIO,     	MUSIC_SCAN_AAC   },
    { ".AMR" , 	RAT_MEDIA_TYPE_AUDIO,      	MUSIC_SCAN_AMR   },
    { ".ATRC"  ,RAT_MEDIA_TYPE_AUDIO,       MUSIC_SCAN_ATRC   },
    { ".MID", 	RAT_MEDIA_TYPE_AUDIO,      	MUSIC_SCAN_MID   },
    { ".WAV", 	RAT_MEDIA_TYPE_AUDIO,      	MUSIC_SCAN_WAV  },

	{ ".JFIF" , RAT_MEDIA_TYPE_PIC,     	PHOTO_SCAN_JFIF  },
    { ".JPEG"  ,RAT_MEDIA_TYPE_PIC,     	PHOTO_SCAN_JPEG   },
    { ".PNG" , 	RAT_MEDIA_TYPE_PIC,      	PHOTO_SCAN_PNG   },
    { ".BMP"  , RAT_MEDIA_TYPE_PIC,         PHOTO_SCAN_BMP   },
    { ".JPG", 	RAT_MEDIA_TYPE_PIC,      	PHOTO_SCAN_JPG   },
    { ".GIF", 	RAT_MEDIA_TYPE_PIC,      	PHOTO_SCAN_GIF  },

    { ".IMG", 	RAT_MEDIA_TYPE_FIRMWARE,    IMAGE_SCAN_IMG  },
        

    { NULL,  	RAT_MEDIA_TYPE_UNKNOWN,  		0			    }
};

__bool __ValidFileType(const char *file, __u64 Types, int *Format)
{
	char    	 	*Suffix;
    FmtIdx_t 	*pFmt;

    if(NULL == file) {
        return EPDK_FALSE;
    }
    
    /* seek to extended name */
    Suffix = eLIBs_strchrlast((char *)file, '.');
    if (NULL == Suffix) {
    	return EPDK_FALSE;
    }   
    
    /* look in ebook valid format table */
    pFmt = ValidFmtTbl;
    while (pFmt->Suffix)
    {
    	if(eLIBs_stricmp(Suffix, pFmt->Suffix) == 0)
        {
            if ((pFmt->ScanFlag & Types) || 0 == Types)
            {
            	*Format = pFmt->Format;
            	return EPDK_TRUE;
        	}
            else
            {
        		return EPDK_FALSE;
        	}
        }
        pFmt++;
    }

    if(0 == Types)//RAT_MEDIA_TYPE_ALL
    {
        *Format = RAT_MEDIA_TYPE_UNKNOWN;
        return EPDK_TRUE;
    }
    
    return EPDK_FALSE;   
}

static void __DestoryScanInfo(ScanInfo_t *pScanInfo)
{
	ScanFileList_t	*pFree;
	ScanFileList_t	*pNextFree;
	
	/* close current */
	if (pScanInfo->pCurDir) {
		eLIBs_closedir(pScanInfo->pCurDir);
	}
	
	/* free scan stack */
	ScanStackFree(pScanInfo->hStack);
	
	/* free scaned file lists of pScanInfo*/
	pFree = pScanInfo->FirstList;
	while (pFree && pScanInfo->CurCnt--) //modify by hot.lee
	{
		pNextFree = pFree->Next;
		rat_free(pFree);
		pFree = pNextFree;
	}
	
	/* free pScanInfo last */
	rat_free(pScanInfo);
}
