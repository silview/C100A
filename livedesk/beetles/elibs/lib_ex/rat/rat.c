#include "epdk.h"
#include "rat_common.h"
#include "ScanPri.h"
#include "Scan.h"
#include "rat_npl.h"
#include "rat.h"
#include "rat_partition.h"

extern __rat_list_t* g_rat_npl[RAT_MEDIA_TYPE_MAX];
extern __rat_list_t g_null_rat_npl;

#define RAT_MAX_LIST_CNT	(RAT_MEDIA_TYPE_MAX*2)

__rat_list_t g_rat_list[RAT_MAX_LIST_CNT] = {0};

__krnl_event_t* g_rat_sem = NULL;

static __bool has_init = 0;
__u8 fav_exit;

#if 0
#define DebugPrintf(...)    		(eLIBs_printf("MSG:L%d(%s):", __LINE__, __FILE__),                 \
							     eLIBs_printf(__VA_ARGS__)		)
#else
#define DebugPrintf(...) 
#endif

/*
****************************************************************************************************
*
*             							
*
*  Description:
*				-	open the rat process.
*
*  Parameters:
*   pDir		-  	pointer to directory(e:\ f:\....  ).
*   media_type	-  	types of media file.
*   OnceCnt		-  	read items number when read once. 
*					if zero, I will read all items one time.
*
*  Return value:
*   NULL   		-  	setup rat process failed.
*	others		-	handle to rat process.
****************************************************************************************************
*/
HRAT rat_open_ex(rat_root_t rat_root_type, rat_media_type_t media_type, __u32 OnceCnt)
{
    __s32 i;
    HRAT hrat;
    __s32 ret;
    char str_root[RAT_MAX_FULL_PATH_LEN];
    char disk_name [ RAT_MAX_PARTITION ][4];
    char* p;

    __wrn("rat_open_ex, rat_root_type=%d\n", rat_root_type);

    if(RAT_TF == rat_root_type)
    {
        if(/*RAT_MEDIA_TYPE_GAME == media_type
            ||*/RAT_MEDIA_TYPE_VIDEO == media_type
			|| RAT_MEDIA_TYPE_AUDIO == media_type
            ||RAT_MEDIA_TYPE_PIC == media_type)
        {
            ret = rat_get_partition_name(RAT_SD_CARD,  disk_name, 1);        
        }
        else
        {
            ret = rat_get_partition_name(RAT_SD_CARD,  disk_name, 0);        
        }
    }
    else if(RAT_USB == rat_root_type)
    {
        ret = rat_get_partition_name(RAT_USB_DISK,  disk_name, 0);        
    }
    else
    {
        ret = EPDK_FAIL;
    }

    if(EPDK_FAIL == ret)
    {
        return NULL;
    }

    i = 0;
    p = str_root;
    eLIBs_memset(str_root, 0, sizeof(str_root));
    while(i < RAT_MAX_PARTITION && disk_name[i][0])
    {        
        eLIBs_strcpy(p, disk_name[i]);
        __wrn("disk_name[i]=%s\n", disk_name[i]);
        p += eLIBs_strlen(disk_name[i]);
        eLIBs_strcpy(p, "/");
        p += eLIBs_strlen("/"); 
        i++;
    }

    *p = 0;
    
    if(0 == i)
    {
        return NULL;
    }

    hrat = rat_open(str_root, media_type, OnceCnt);
    
    return hrat;
}

HRAT rat_open(const char *pDir, rat_media_type_t media_type, __u32 OnceCnt)
{
	__s32 i;
	HRAT  hrat;
	HSCAN hscan;
	__u64 file_type;
    __bool rat_npl_need_update;
    __bool rat_list_research;
	ScanInfo_t 	*pScanInfo;
    
    rat_list_research = EPDK_FALSE;//ÎÄ¼þÁÐ±íÊÇ·ñ±»ÖØÐÂËÑË÷
    rat_npl_need_update = EPDK_FALSE;//ÎÄ¼þÁÐ±í±»ÖØÐÂËÑË÷ºó£¬²¥·ÅÁÐ±íÊÇ·ñÐèÒª¸üÐÂ
    
    __msg("rat_open: pDir=%s media_type=%d OnceCnt=%d\n", pDir, media_type, OnceCnt);

	RAT_ENTER_CRITICAL_SECTION;
	__msg("************************************\n");
	for (i = 0 ; i < RAT_MAX_LIST_CNT ; i++)
	{
		__msg("************************************\n");
		if (0 == eLIBs_strcmp(pDir, g_rat_list[i].str_path)
			&& media_type == g_rat_list[i].media_type)
		{
			if (0 == g_rat_list[i].open_cnt)
			{
				RAT_LEAVE_CRITICAL_SECTION;
				__msg("error, open_cnt=0\n");
				return NULL;
			}
			if (NULL == g_rat_list[i].hscan)
			{
				RAT_LEAVE_CRITICAL_SECTION;
				__msg("error, g_rat_list[i].hrat=NULL\n");
				return NULL;
			}

            if(0 == g_rat_list[i].modify_flag)//Èç¹û¸ÃÁÐ±íÎ´±»¸´ÖÆ»òÉ¾³ý¹ý£¬ÔòÎÞÐèÖØÐÂËÑË÷
            {
    			g_rat_list[i].open_cnt++;
    			ScanMoveToFirst(g_rat_list[i].hscan);
    			RAT_LEAVE_CRITICAL_SECTION;
    			return (HRAT)&g_rat_list[i];
            }
            else
            {
                rat_list_research = EPDK_TRUE;
                    
                //Èç¹û²¥·ÅÁÐ±íËùÔÚÎÄ¼þÁÐ±í±»ÖØÐÂËÑË÷ÁË£¬ÔòÐè°ÑÎÄ¼þÁÐ±íÍ¬²½µ½²¥·ÅÁÐ±í
                if(NULL != g_rat_npl[media_type]
                    && g_rat_npl[media_type] == &g_rat_list[i])
                {
                    rat_npl_need_update = EPDK_TRUE;
                }
                
                ScanFree(g_rat_list[i].hscan);
				g_rat_list[i].hscan = NULL;
				g_rat_list[i].open_cnt = 0;
				g_rat_list[i].str_path[0] = 0;
                g_rat_list[i].modify_flag = 0;   
                break;
            }
		}
	}
	__msg("************************************\n");

	if (RAT_MEDIA_TYPE_PIC == media_type)
	{
		file_type = PHOTO_SCAN_ALL;
	}
	else if (RAT_MEDIA_TYPE_VIDEO == media_type)
	{
		file_type = MOVIE_SCAN_ALL;
	}
	else if (RAT_MEDIA_TYPE_AUDIO == media_type)
	{
		file_type = MUSIC_SCAN_ALL;
	}
	else if (RAT_MEDIA_TYPE_EBOOK == media_type)
	{
		file_type = EBOOK_SCAN_ALL;
	}
    else if (RAT_MEDIA_TYPE_FAVOR_PIC == media_type)
	{
		file_type = PHOTO_SCAN_FAVOR;
	}
	else if (RAT_MEDIA_TYPE_FAVOR_VIDEO == media_type)
	{
		file_type = MOVIE_SCAN_FAVOR;
	}
	else if (RAT_MEDIA_TYPE_FAVOR_AUDIO == media_type)
	{
		file_type = MUSIC_SCAN_FAVOR;
	}
	else if (RAT_MEDIA_TYPE_FAVOR_EBOOK == media_type)
	{
		file_type = EBOOK_SCAN_FAVOR;
	}
    else if(RAT_MEDIA_TYPE_ALL == media_type)
    {
        file_type = 0;
    }
	else
	{
		RAT_LEAVE_CRITICAL_SECTION;
		return NULL;
	}

	__wrn("pDir=%s\n",pDir);

	hscan = ScanFilesBegin(pDir, file_type, OnceCnt);
	pScanInfo = (ScanInfo_t*)hscan;

	__wrn("pScanInfo->CurCnt =%d\n",pScanInfo->CurCnt );
	if(pScanInfo->CurCnt == 0)
			fav_exit =1;
	else
			fav_exit =0;
    rat_set_search_cb(NULL, NULL);
	if (NULL == hscan)
	{
		RAT_LEAVE_CRITICAL_SECTION;
		__msg("ScanFilesBegin fail...\n");
		return NULL;
	}

    //ÎÄ¼þÁÐ±í±»ÖØÐÂËÑË÷ºó£¬ÆäÁ´±íÊ×µØÖ·±»´æ´¢µÄÎ»ÖÃ²»±ä£¬²»ÐèÒªÖØÐÂ²éÕÒ´æ´¢Î»ÖÃ
    //ÕâÑù¿ÉÒÔ±ÜÃâÎÄ¼þÁÐ±íÖØÐÂËÑË÷ºó£¬²¥·ÅÁÐ±íÃ»ÓÐ±»Í¬²½µÄÎÊÌâ
    if(EPDK_FALSE == rat_list_research)
    {
    	for (i = 0 ; i < RAT_MAX_LIST_CNT ; i++)
    	{
    		if (NULL == g_rat_list[i].hscan
    			&& 0 == g_rat_list[i].open_cnt)
    		{
    			break;
    		}
    	}
    }

	if (i >= RAT_MAX_LIST_CNT)
	{
		__msg("no free space for rat list...\n");
		ScanFree(hscan);
		RAT_LEAVE_CRITICAL_SECTION;
		return NULL;
	}

	ScanMoveToFirst(hscan);
    __here__;

	g_rat_list[i].hscan = hscan;
    __here__;
	g_rat_list[i].media_type = media_type;
    __here__;

    //Èç¹ûÁÐ±í±»ÖØÐÂËÑË÷£¬ËµÃ÷²»ÊÇµÚÒ»´ÎËÑË÷£¬ÒýÓÃ¼ÆÊý¼Ó1¡£
    //·ñÔòÒýÓÃ¼ÆÊýÎª1
    if(EPDK_FALSE == rat_list_research)
    {
	    g_rat_list[i].open_cnt = 1;
    }
    else
    {
        g_rat_list[i].open_cnt++;
    }

    if(g_rat_list[i].open_cnt < 1)//±£»¤Ò»ÏÂ
    {
        g_rat_list[i].open_cnt = 1;
    }
    __here__;
	eLIBs_strcpy(g_rat_list[i].str_path, pDir);
    __here__;

    //Èç¹û²¥·ÅÁÐ±í±»Í¬²½£¬ÔòÐèÖØÐÂÉè¶¨ÕýÈ·µÄnpl_cur_index
    if(EPDK_TRUE == rat_npl_need_update)
    {
        __s32 total_cnt;

        total_cnt = ScanGetCurCnt(hscan);
        
        //g_rat_list[i].npl_cur_index = 0;	//²»ÔÚÕâÀï×ö£¬ÓÉÓ¦ÓÃ³ÌÐò´ÓrobinÈ¡µÃÎÄ¼þÃû£¬ÖØÐÂµ÷ÓÃset_file_for_play
        
        if(g_rat_list[i].npl_cur_index > total_cnt-1)
        {
            g_rat_list[i].npl_cur_index = total_cnt-1;
        }

        if(g_rat_list[i].npl_cur_index < 0)
        {
            g_rat_list[i].npl_cur_index = 0;
        }
    }
    else
    {
        g_rat_list[i].npl_cur_index = 0;	
    }	

    __here__;
	//ÔÚ·µ»ØÖ®Ç°ÅÅÐòÁÐ±í add by hot.lee
	/*if(pScanInfo->CurCnt > 1)
	{
		rat_file_list_sort(pScanInfo,pScanInfo->CurCnt);

	}*/
	RAT_LEAVE_CRITICAL_SECTION;   

	return (HRAT)&g_rat_list[i];
}

/*
****************************************************************************************************
*
*             							
*
*  Description:
*				-	move rat scan handle cursor position to first entry.
*				  	
*  Parameters:
*	hRat		-	handle to rat information.
*
*  Return value:
*   EPDK_OK   	-  	move succeeded.
*	EPDK_FAIL	-	move failed.
****************************************************************************************************
*/
__s32 rat_move_cursor_to_first(HRAT hRat)
{
	__s32 ret; 

	__rat_list_t* pRatList;
	
	pRatList = (__rat_list_t*)hRat;
	if (0 == pRatList || 0 == pRatList->hscan)
	{
		return -1;
	}
	RAT_ENTER_CRITICAL_SECTION;
	ret = ScanMoveToFirst(pRatList->hscan);
	RAT_LEAVE_CRITICAL_SECTION;
	return ret;
}

/*
****************************************************************************************************
*
*             							
*
*  Description:
*				-	move rat handle cursor position forward.
*				  	
*  Parameters:
*	hRat		-	handle to rat information.
*
*  Return value:
*   __u32   	-  	count of cursor has actually move.
*	
****************************************************************************************************
*/
__u32 rat_move_cursor_forward(HRAT hRat, __u32 cnt)
{
    __u32 i;    
	__s32 ret;     

	__rat_list_t* pRatList;
	
	pRatList = (__rat_list_t*)hRat;
	if (0 == pRatList || 0 == pRatList->hscan)
	{
		return 0;
	}

    i = 0;
	RAT_ENTER_CRITICAL_SECTION;
    while(!ScanIsDone(pRatList->hscan))
    {
        if(i >= cnt)
        {
            break;
        }
        ret = ScanMoveToNext(pRatList->hscan);
        if(EPDK_FAIL == ret)
        {
            break;
        }
        i++;
    }
	
	RAT_LEAVE_CRITICAL_SECTION;
	return i;
}

/*
****************************************************************************************************
*
*             							
*
*  Description:
*				-	move rat handle cursor backward
*				  	
*  Parameters:
*	hRat		-	handle to rat information.
*
*  Return value:
*   __u32   	-  	count of cursor has actually move.
*	
****************************************************************************************************
*/
__u32 rat_move_cursor_backward(HRAT hRat, __u32 cnt)
{
    __u32 i;
	__s32 ret; 
	__rat_list_t* pRatList;
	
	pRatList = (__rat_list_t*)hRat;
	if (0 == pRatList || 0 == pRatList->hscan)
	{
		return 0;
	}
	i = 0;
	RAT_ENTER_CRITICAL_SECTION;
    while(i < cnt)
    {
	    ret = ScanMoveToPrev(pRatList->hscan);
        if(EPDK_FAIL == ret)
        {
            break;
        }
        i++;
    }
	RAT_LEAVE_CRITICAL_SECTION;
	return i;
}

/*
****************************************************************************************************
*
*             							
*
*  Description:
*				-	check npl handle cursor position pointing after the end entry.
*				  	
*  Parameters:
*	hRat		-	handle to npl information.
*
*  Return value:
*   EPDK_TRUE   -  	cursor position pointing the end entry.
*	EPDK_FALSE	-	cursor position not pointing at the end entry.
****************************************************************************************************
*/
__bool rat_is_cursor_end(HRAT hRat)
{
	__s32 ret; 

	__rat_list_t* pRatList;
	
	pRatList = (__rat_list_t*)hRat;
	if (0 == pRatList || 0 == pRatList->hscan)
	{
		return -1;
	}
	
	RAT_ENTER_CRITICAL_SECTION;
	ret = ScanIsDone(pRatList->hscan);
	RAT_LEAVE_CRITICAL_SECTION;
	return ret;
}

/*
****************************************************************************************************
*
*             							
*
*  Description:
*				-	free npl handle.
*
*  Parameters:
*	hRat		-	handle to npl information.
*
*  Return value:
*   	   		NONE
****************************************************************************************************
*/
__s32 rat_close(HRAT hRat)
{
	__s32 i;
	__s32 ret; 
	__rat_list_t* pRatList;
	
	pRatList = (__rat_list_t*)hRat;
	if (0 == pRatList || 0 == pRatList->hscan)
	{
		return -1;
	}

	RAT_ENTER_CRITICAL_SECTION;
	
	for (i = 0 ; i < RAT_MAX_LIST_CNT ; i++)
	{
		if (pRatList->hscan == g_rat_list[i].hscan)
		{
			g_rat_list[i].open_cnt--;
			if (0 == g_rat_list[i].open_cnt)
			{
				ScanFree((HSCAN)hRat);
				g_rat_list[i].hscan = NULL;
				g_rat_list[i].open_cnt = 0;
				g_rat_list[i].str_path[0] = 0;
                g_rat_list[i].modify_flag = 0;
				if (g_rat_npl[g_rat_list[i].media_type] == pRatList)//É¾³ý²¥·ÅÁÐ±í
				{
					g_rat_npl[g_rat_list[i].media_type] = NULL;
				}
			}
			break;
		}
	}
	if (i >= RAT_MAX_LIST_CNT)
	{
		__msg("error: hrat not found in list...\n");
		RAT_LEAVE_CRITICAL_SECTION;
		return -1;
	}
	RAT_LEAVE_CRITICAL_SECTION;

	return 0;
}

/*
****************************************************************************************************
*
*             							
*
*  Description:
*				-	get current npled file entry.
*				  	
*  Parameters:
*	hRat		-	handle to npl information.
*	entry		-	to store entry info
*  Return value:
*   EPDK_OK   		-  	get current entry ok.
*	EPDK_FAIL		-	get current entry failed.
****************************************************************************************************
*/
__s32 rat_get_cur_item_info(HRAT hRat, rat_entry_t* entry)
{
	__s32 ret; 
	__rat_list_t* pRatList;
	rat_entry_t* pEntry; 
	pRatList = (__rat_list_t*)hRat;
	if (0 == pRatList || 0 == pRatList->hscan)
	{
		return EPDK_FAIL;
	}

	RAT_ENTER_CRITICAL_SECTION;
	pEntry = ScanGetCurrent(pRatList->hscan);	
    if(pEntry)
    {
	    eLIBs_memcpy(entry, pEntry, sizeof(rat_entry_t));
        ret = EPDK_OK;
    }
    else
    {
        ret = EPDK_FAIL;
    }
	RAT_LEAVE_CRITICAL_SECTION;
	
	return ret;
}

/*
****************************************************************************************************
*
*             							
*
*  Description:
*				-	set current file to be play.
*				  	
*  Parameters:
*	hRat		-	handle to rat.
*	file		-   full file path
*
*  Return value:
*   EPDK_FAIL   		-  	no such file
*   EPDK_TRUE   		-  	ok
*	others		-	
****************************************************************************************************
*/
__s32 rat_set_file_for_play(HRAT hRat, char* file)
{
	__s32 i;
	__s32 ret; 
	__rat_list_t* pRatList;	
	HRATNPL hnpl;
	pRatList = (__rat_list_t*)hRat;	
	if (0 == pRatList || 0 == pRatList->hscan)
	{
		return -1;
	}
	
	RAT_ENTER_CRITICAL_SECTION;
	
	g_rat_npl[pRatList->media_type] = pRatList;

	RAT_LEAVE_CRITICAL_SECTION;
	
	//ÉèÖÃµ±Ç°²¥·ÅÌõÄ¿
	DebugPrintf("------pRatList->media_type=%d--------\n",pRatList->media_type);
	hnpl = rat_npl_open(pRatList->media_type);
	if (NULL != hnpl)
	{
		DebugPrintf("-------NULL != hnpl--------\n");
		ret = rat_npl_file2index(hnpl, file);
		DebugPrintf("------ret=%d--------\n",ret);
		if (-1 == ret)
		{
			ret = 0;
		}

		rat_npl_set_cur(hnpl, ret);
		rat_npl_close(hnpl);
	}
	return 0;
}

/*
****************************************************************************************************
*
*             							
*
*  Description:
*				-	get current item info by index.
*				  	
*  Parameters:
*	hRat		-	handle to rat.
*	index		-   zero base index
*	entry		-	to store entry info
*  Return value:
*   EPDK_OK		   		-  	current item info
*   EPDK_FAIL	   		-  	no such item
*	others		-	
****************************************************************************************************
*/
__s32 rat_get_item_info_by_index(HRAT hRat, __s32 index, rat_entry_t* entry)
{
	__rat_list_t* rat_list;
	ScanInfo_t* pScanInfo;
	rat_entry_t* pEntry; 
	__s32 n;
    __s32 ret;

    DebugPrintf("index=%d\n", index);
    __wrn("index=%d\n", index);
	
    
	if (NULL == hRat)
	{
        __here__;
		DebugPrintf("hRat is NULL\n");
		return EPDK_FAIL;
	}		
	
	RAT_ENTER_CRITICAL_SECTION;
	
	rat_list = (__rat_list_t*)hRat;
	if (NULL == rat_list->hscan)
	{
        __here__;
		DebugPrintf("rat_list->hscan is NULL\n");
		RAT_LEAVE_CRITICAL_SECTION;
		return EPDK_FAIL;
	}
	
	pScanInfo = (ScanInfo_t*)(rat_list->hscan);
	
	if (pScanInfo->CurCnt <= 0 || index >= pScanInfo->CurCnt)
	{
        DebugPrintf("pScanInfo->CurCnt=%d\n", pScanInfo->CurCnt);
		RAT_LEAVE_CRITICAL_SECTION;
		return EPDK_FAIL;
	}
	
	__ScanMoveCursorToFirst(rat_list->hscan, &pScanInfo->TmpCursor);
	n = 0;
	while (n < index)
	{
		if (EPDK_OK != __ScanMoveCursorToNext(rat_list->hscan, &pScanInfo->TmpCursor))
		{
			break;
		}
		n++;
	}
		DebugPrintf("n=%d index=%d \n",n,index);
	if (n == index)
	{
		pEntry = __ScanGetCurrentCursorData(rat_list->hscan, &pScanInfo->TmpCursor);
        if(pEntry)
        {
		    eLIBs_memcpy(entry, pEntry, sizeof(rat_entry_t));
            ret = EPDK_OK;
        }
        else
        {
            __here__;
		DebugPrintf("pEntry is NULL\n");
            ret = EPDK_FAIL;
        }
		RAT_LEAVE_CRITICAL_SECTION;
		return ret;
	}
	
	RAT_LEAVE_CRITICAL_SECTION;
	return EPDK_FAIL;
}

/*
****************************************************************************************************
*
*             							
*
*  Description:
*				-	get current item info by index.
*				  	
*  Parameters:
*	hRat		-	handle to rat.
*	index		-   zero base index
*
*  Return value:
*   rat_entry_t*   		-  	current item info
*   NULL		   		-  	no such item
*	others		-	
****************************************************************************************************
*/
__u32 rat_get_cur_scan_cnt(HRAT hRat)
{	
	__rat_list_t* pRatList;
	ScanInfo_t *pScanInfo;
	__u32 cnt;

	pRatList = (__rat_list_t*)hRat;
	if (0 == pRatList || 0 == pRatList->hscan)
	{
		return 0;
	}
	
	RAT_ENTER_CRITICAL_SECTION;
	pScanInfo = (ScanInfo_t *)(pRatList->hscan);
	cnt = pScanInfo->CurCnt;
	RAT_LEAVE_CRITICAL_SECTION;

	return cnt;
}

__s32 rat_init(void)
{
	if (1 == has_init)
	{
		return EPDK_OK;	
	}

	if (0 == g_rat_sem)
	{
		g_rat_sem = esKRNL_SemCreate(1);
	}

	eLIBs_memset(g_rat_list, 0, sizeof(g_rat_list));
	eLIBs_memset(&g_null_rat_npl, 0, sizeof(g_null_rat_npl));

    rat_set_search_cb(NULL, NULL);

	has_init = 1;	
	
	return EPDK_OK;
}

__s32 rat_deinit(void)
{
	__u8    err;

	if (0 == has_init)
	{
		return EPDK_OK;
	}

    rat_clear_all_list();
	
	if (g_rat_sem)
	{		
		esKRNL_SemDel(g_rat_sem, OS_DEL_ALWAYS, &err);		
		g_rat_sem = NULL;
	}

    rat_set_search_cb(NULL, NULL);

	has_init = 0;
	
	return EPDK_OK;
}

//clear all rat list and npl
__s32 rat_clear_all_list_by_root(void* arg)
{
    __s32 i;
    char str_cur[2]={0};

    str_cur[0] = (char)arg;
    RAT_ENTER_CRITICAL_SECTION;

    __msg("rat_clear_all_list, str_cur=%s\n", str_cur);
    
    //É¾³ý²¥·ÅÁÐ±í
    for(i = 0 ; i < RAT_MEDIA_TYPE_MAX ; i++)
    {
        if(g_rat_npl[i])
        {
            __msg("g_rat_npl[%d]->str_path=%s\n", i, g_rat_npl[i]->str_path);
            
            if(0 == eLIBs_strnicmp(str_cur, g_rat_npl[i]->str_path, 1))
            {
                __msg("clear rat npl:g_rat_npl[%d]->str_path=%s\n", i, g_rat_npl[i]->str_path);
                g_rat_npl[i] = NULL;            
            }
        }
        else
        {
            __msg("g_rat_npl[%d] is null\n", i);
        }
    }

    __here__;

    //É¾³ýÎÄ¼þÁÐ±í
    for (i = 0 ; i < RAT_MAX_LIST_CNT ; i++)
	{
		if (g_rat_list[i].hscan)
		{            
            __msg("g_rat_list[%d].str_path=%s\n", i, g_rat_list[i].str_path);
            
            if(0 == eLIBs_strnicmp(str_cur, g_rat_list[i].str_path, 1))
            {
                __msg("clear rat list,g_rat_list[%d].str_path=%s\n", i, g_rat_list[i].str_path);
    			ScanFree(g_rat_list[i].hscan);
    			g_rat_list[i].hscan = NULL;
    			g_rat_list[i].open_cnt = 0;
    			g_rat_list[i].str_path[0] = 0;	                
                g_rat_list[i].modify_flag = 0;
            }
		}
        else
        {
            __msg("g_rat_list[%d].hscan is null\n", i);
        }
	}
    __here__;
    RAT_LEAVE_CRITICAL_SECTION;
    
    return 0;
}


//clear all rat list and npl
__s32 rat_clear_all_list(void)
{
    __s32 i;
    
    RAT_ENTER_CRITICAL_SECTION;

    __msg("rat_clear_all_list\n");
    
    //É¾³ý²¥·ÅÁÐ±í
    for(i = 0 ; i < RAT_MEDIA_TYPE_MAX ; i++)
    {
        if(g_rat_npl[i])
        {
            __msg("g_rat_npl[%d]->str_path=%s\n", i, g_rat_npl[i]->str_path);
            
            //if(0 == eLIBs_strnicmp(str_cur, g_rat_npl[i]->str_path, 1))
            {
                __msg("clear rat npl:g_rat_npl[%d]->str_path=%s\n", i, g_rat_npl[i]->str_path);
                g_rat_npl[i] = NULL;            
            }
        }
        else
        {
            __msg("g_rat_npl[%d] is null\n", i);
        }
    }

    __here__;

    //É¾³ýÎÄ¼þÁÐ±í
    for (i = 0 ; i < RAT_MAX_LIST_CNT ; i++)
	{
		if (g_rat_list[i].hscan)
		{            
            __msg("g_rat_list[%d].str_path=%s\n", i, g_rat_list[i].str_path);
            
            //if(0 == eLIBs_strnicmp(str_cur, g_rat_list[i].str_path, 1))
            {
                __msg("clear rat list,g_rat_list[%d].str_path=%s\n", i, g_rat_list[i].str_path);
    			ScanFree(g_rat_list[i].hscan);
    			g_rat_list[i].hscan = NULL;
    			g_rat_list[i].open_cnt = 0;
    			g_rat_list[i].str_path[0] = 0;	
                g_rat_list[i].modify_flag = 0;
            }
		}
        else
        {
            __msg("g_rat_list[%d].hscan is null\n", i);
        }
	}
    __here__;
    RAT_LEAVE_CRITICAL_SECTION;
    
    return 0;
}


rat_media_type_t rat_get_file_type(const char* file_name)
{
	rat_media_type_t media_type = RAT_MEDIA_TYPE_UNKNOWN;

	__ValidFileType(file_name, 0, (int *)(&media_type));
	
	return media_type;
}

__s32 rat_set_modify_flag(rat_media_type_t media_type, __s32 flag)
{
    __s32 i;

    __msg("rat_set_modify_flag: media_type=%d, flag = %d\n", media_type, flag);

    RAT_ENTER_CRITICAL_SECTION;

    for (i = 0 ; i < RAT_MAX_LIST_CNT ; i++)
    {
        if (g_rat_list[i].hscan
            && media_type == g_rat_list[i].media_type)
        {                       
            g_rat_list[i].modify_flag = flag;
        }
    }
    
    RAT_LEAVE_CRITICAL_SECTION

    return EPDK_OK;
}

__s32 rat_set_modify_flag_all(__s32 flag)
{
    __s32 i;

    __msg("rat_set_modify_flag_all: flag = %d\n", flag);

    RAT_ENTER_CRITICAL_SECTION;

    for (i = 0 ; i < RAT_MAX_LIST_CNT ; i++)
    {
        if (g_rat_list[i].hscan)
        {                       
            g_rat_list[i].modify_flag = flag;
        }
    }
    
    RAT_LEAVE_CRITICAL_SECTION

    return EPDK_OK;
}

__s32 rat_set_search_cb(prat_search_cb cb, void* arg)
{
    g_rat_search_cb = cb;
    g_rat_search_arg = arg;
    return EPDK_OK;
}

__s32 rat_add_favor_file(HRAT hrat, char* filename)
{
	__rat_list_t* pRatList;
	ScanInfo_t *pScanInfo;
	__u32 cnt;
    __s32 bok;
    ES_FILE* fp;
    
	DebugPrintf("rat_add_favor_file...\n");

	pRatList = (__rat_list_t*)hrat;
	if (0 == pRatList || 0 == pRatList->hscan)
	{
		DebugPrintf("EPDK_FAIL\n");
		return EPDK_FAIL;
	}

    if(NULL == filename)
    {
        __err("invalid para...\n");
		DebugPrintf("invalid para\n");
        return EPDK_FAIL;
    }

    
    bok = EPDK_OK;    
    fp = NULL;  
	
	RAT_ENTER_CRITICAL_SECTION;
	pScanInfo = (ScanInfo_t *)(pRatList->hscan);

    if(RAT_MEDIA_TYPE_FAVOR_AUDIO != pRatList->media_type
        &&RAT_MEDIA_TYPE_FAVOR_VIDEO != pRatList->media_type
        &&RAT_MEDIA_TYPE_FAVOR_PIC != pRatList->media_type
        &&RAT_MEDIA_TYPE_FAVOR_EBOOK != pRatList->media_type)
    {
        DebugPrintf("invalid media type...\n");
        RAT_LEAVE_CRITICAL_SECTION;
        return EPDK_FAIL;
    }

    do{//´¦ÀíÊÕ²ØÀàÐÍÉ¨Ãè
        __s32 ret;
        __s32 cnt;
        __s32 val;
        __s32 num;
        __s32 i;
        __s32 fsize;
                
        char str_db_name[32]={0};
        char str_db_path[1024]={0};
        char str_file_item[1024]={0};

		DebugPrintf("start_add_to_rat\n");
        cnt = 0;              
        
        if(PHOTO_SCAN_FAVOR == pScanInfo->ScanTypes)
        {
            eLIBs_strcpy(str_db_name, "photo.fdb");
        }
        else if(MUSIC_SCAN_FAVOR == pScanInfo->ScanTypes)
        {
			DebugPrintf("MUSIC_SCAN_FAVOR\n");
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
			DebugPrintf("bok = EPDK_FAIL\n");
            bok = EPDK_FAIL;
            break;
        }

        eLIBs_sprintf(str_db_path, "%c:\\%s%s", filename[0],
            "favor_db\\", str_db_name);
        DebugPrintf("str_db_path=%s\n", str_db_path);

        {//make dir
            ES_DIR		*tmp_dir;
            char str_tmp[1024]={0};
            
            eLIBs_sprintf(str_tmp, "%c:\\%s", filename[0],
            "favor_db");
        
            tmp_dir = eLIBs_opendir(str_tmp);
        	if(!tmp_dir)
        	{
        		if(eLIBs_mkdir(str_tmp))
        		{      
                    DebugPrintf("create db dir fail, str_tmp=%s...\n", str_tmp);
        		}
                else
                {
                    DebugPrintf("create db dir success, str_tmp=%s...\n", str_tmp);
                }
        	}
        	else
        	{
        		eLIBs_closedir(tmp_dir);
        		tmp_dir = 0;
        	}
        }

        fp = eLIBs_fopen(str_db_path, "rb+");
        if(NULL == fp)
        {
            __wrn("db not exist, create it...\n");
            fp = eLIBs_fopen(str_db_path, "wb");
            if(fp)
            {
                eLIBs_fclose(fp);
                fp = NULL;
                
                __wrn("create db success...\n");
            }
            else
            {
                __wrn("create db fail...\n");
            }
            
            fp = eLIBs_fopen(str_db_path, "rb+");
        }
        else
        {
            __wrn("db exist...\n");
        }
                
        if(NULL == fp)
        {
            bok = EPDK_FAIL;
          DebugPrintf("file does not exist,str_db_path=%s\n", str_db_path);
            goto end;
        }

        DebugPrintf("db exist or create db success,str_db_path=%s\n", str_db_path);
        ret = eLIBs_fread(&val, 1, 4, fp);
        if(ret != 0 && ret < 4)
        {
            bok = EPDK_FAIL;
            DebugPrintf("eLIBs_fread fail...\n");
            goto end;
        }

        if(0 == ret)//³õÊ¼´´½¨Êý¾ÝÎÄ¼þ
        {
            char tmp[16]={0};
            DebugPrintf("item count is 0, begin create db...\n");
            val = 0;
            num = 0;

            eLIBs_fseek(fp, 0, SEEK_SET);
            eLIBs_fwrite(tmp, 1, 16, fp);
            goto save_item;
        }

        fsize = val;
        DebugPrintf("fsize=%d\n", fsize);
        eLIBs_fseek(fp, 4, SEEK_SET);
        ret = eLIBs_fread(&val, 1, 4, fp);
        if(4 != ret)
        {
            DebugPrintf("eLIBs_fread fail...\n");
            bok = EPDK_FAIL;
            goto end;
        }

        num = val;
		DebugPrintf("num=%d...\n",num);

        eLIBs_fseek(fp, 0, SEEK_END);
        ret = eLIBs_ftell(fp);
        if(ret < num*1024+16
            || ret < fsize)
        {
            bok = EPDK_FAIL;
            __wrn("verify db fail...\n");
            goto del_db;
        }

        eLIBs_fseek(fp, 16, SEEK_SET);
        
        
        for(i = 0 ; i < num ; i++)
        {
            ret = eLIBs_fread(str_file_item, 1, 1024, fp);
            DebugPrintf("str_file_item=%s...\n",str_file_item);
            if(1024 != ret)
            {
                bok = EPDK_FAIL;
               DebugPrintf("eLIBs_fread fail...\n");
                goto end;
            }

            str_file_item[0] = filename[0];//ÐÞ¸ÄÂ·¾¶ÅÌ·û
            ret = eLIBs_stricmp(str_file_item, filename);
            if(0 == ret)
            {
                bok = EPDK_OK;
                DebugPrintf("identify str found...\n");
                goto end;
            }            
        }        

        save_item:;
                
        eLIBs_memset(str_file_item, 0, sizeof(str_file_item));
        eLIBs_strcpy(str_file_item, filename);
		DebugPrintf("str_file_item=%s...\n",str_file_item);
		DebugPrintf("save_item...\n");

        eLIBs_fseek(fp, 16+num*1024, SEEK_SET);
        
        ret = eLIBs_fwrite(str_file_item, 1, 1024, fp);
        if(1024 != ret)
        {
            bok = EPDK_FAIL;
            DebugPrintf("eLIBs_fwrite fail...\n");
            goto end;   
        }
        
        eLIBs_fseek(fp, 4, SEEK_SET);
        num++;
        eLIBs_fwrite(&num, 1, 4, fp);
        DebugPrintf("add item success, item num=%d\n", num);        
        eLIBs_fseek(fp, 0, SEEK_SET);
        fsize = num*1024+16;
        eLIBs_fwrite(&fsize, 1, 4, fp);
        DebugPrintf("modify fsize success, fsize=%d\n", fsize);  
        
        end:;
        if(fp)
        {
            eLIBs_fclose(fp);
            fp = NULL;
        }

		break;

        del_db:;
        if(fp)
        {
            eLIBs_fclose(fp);
            fp = NULL;
        }

        eLIBs_remove(str_db_path);
        break;
        
    }while(0);

    RAT_LEAVE_CRITICAL_SECTION;
    
    return bok;       
}

__s32 rat_del_favor_file(HRAT hrat, char* filename)
{
    __rat_list_t* pRatList;
	ScanInfo_t *pScanInfo;
	__u32 cnt;
    __s32 bok;
    ES_FILE* fp;
    

	pRatList = (__rat_list_t*)hrat;
	if (0 == pRatList || 0 == pRatList->hscan)
	{
		return EPDK_FAIL;
	}

    if(NULL == filename)
    {
        DebugPrintf("invalid para...\n");
        return EPDK_FAIL;
    }

    
    bok = EPDK_OK;   
    fp = NULL;  
	
	RAT_ENTER_CRITICAL_SECTION;
	pScanInfo = (ScanInfo_t *)(pRatList->hscan);

    if(RAT_MEDIA_TYPE_FAVOR_AUDIO != pRatList->media_type
        &&RAT_MEDIA_TYPE_FAVOR_VIDEO != pRatList->media_type
        &&RAT_MEDIA_TYPE_FAVOR_PIC != pRatList->media_type
        &&RAT_MEDIA_TYPE_FAVOR_EBOOK != pRatList->media_type)
    {
        DebugPrintf("invalid media type...\n");
        RAT_LEAVE_CRITICAL_SECTION;
        return EPDK_FAIL;
    }

    do{//´¦ÀíÊÕ²ØÀàÐÍÉ¨Ãè
        __s32 ret;
        __s32 cnt;
        __s32 val;
        __s32 num;
        __s32 i;
        __s32 fsize;
                
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
            bok = EPDK_FAIL;
            break;
        }

        eLIBs_sprintf(str_db_path, "%c:\\%s%s", filename[0],
            "favor_db\\", str_db_name);

        fp = eLIBs_fopen(str_db_path, "rb+");
        if(NULL == fp)
        {
            fp = eLIBs_fopen(str_db_path, "wb");
            if(fp)
            {
                eLIBs_fclose(fp);
                fp = NULL;
            }
            
            fp = eLIBs_fopen(str_db_path, "rb+");
        }
        
        if(NULL == fp)
        {
            bok = EPDK_FAIL;
            DebugPrintf("file does not exist,str_db_path=%s\n", str_db_path);
            goto end;
        }

        ret = eLIBs_fread(&val, 1, 4, fp);
        if(ret != 0 && ret < 4)
        {
            bok = EPDK_FAIL;
            DebugPrintf("eLIBs_fread fail...\n");
            goto end;
        }

        if(0 == ret)//³õÊ¼´´½¨Êý¾ÝÎÄ¼þ
        {
            char tmp[16]={0};
            DebugPrintf("item count is 0, begin create db...\n");
            val = 0;
            num = 0;

            eLIBs_fseek(fp, 0, SEEK_SET);
            eLIBs_fwrite(tmp, 1, 16, fp);
            goto save_item;
        }

        fsize = val;
        DebugPrintf("fsize=%d\n", fsize);

        eLIBs_fseek(fp, 4, SEEK_SET);
        ret = eLIBs_fread(&val, 1, 4, fp);
        if(4 != ret)
        {
            DebugPrintf("eLIBs_fread fail...\n");
            bok = EPDK_FAIL;
            goto end;
        }

        num = val;
        DebugPrintf("num=%d\n", num);

        eLIBs_fseek(fp, 0, SEEK_END);
        ret = eLIBs_ftell(fp);
        if(ret < num*1024+16
            || ret < fsize)
        {
            bok = EPDK_FAIL;
            DebugPrintf("eLIBs_fread fail...\n");
            goto del_db;
        }

        eLIBs_fseek(fp, 16, SEEK_SET);
        
        
        for(i = 0 ; i < num ; i++)
        {
            ret = eLIBs_fread(str_file_item, 1, 1024, fp);
            if(1024 != ret)
            {
                bok = EPDK_FAIL;
                DebugPrintf("eLIBs_fread fail...\n");
                goto end;
            }

            str_file_item[0] = filename[0];//ÐÞ¸ÄÂ·¾¶ÅÌ·û
            ret = eLIBs_stricmp(str_file_item, filename);
            if(0 == ret)
            {
                __s32 j;
                
                DebugPrintf("identify str found, begin to del...\n");
                for(j = i ; j < num-1 ; j++)
                {
                    eLIBs_fseek(fp, 16+(j+1)*1024, SEEK_SET);
                    eLIBs_fread(str_file_item, 1, 1024, fp);
                    eLIBs_fseek(fp, 16+j*1024, SEEK_SET);
                    eLIBs_fwrite(str_file_item, 1, 1024, fp);                    
                }
                
                num--;
                
                break;
            }            
        }        
              
        eLIBs_fseek(fp, 4, SEEK_SET);
        if(num < 0)
        {
            num = 0;
        }
        eLIBs_fwrite(&num, 1, 4, fp);
        DebugPrintf("add item success, item num=%d\n", num);        

        eLIBs_fseek(fp, 0, SEEK_SET);
        fsize = num*1024+16;
        eLIBs_fwrite(&fsize, 1, 4, fp);
        DebugPrintf("modify fsize success, fsize=%d\n", fsize);  

        save_item:;
        
        end:;
        if(fp)
        {
            eLIBs_fclose(fp);
            fp = NULL;
        }

        break;
        del_db:;
        if(fp)
        {
            eLIBs_fclose(fp);
            fp = NULL;
        }

        eLIBs_remove(str_db_path);
        
    }while(0);

    RAT_LEAVE_CRITICAL_SECTION;
    
    return bok;       
    
}

//0:²»ÊÇÊÕ²ØÎÄ¼þ£¬1:ÊÇÊÕ²ØÎÄ¼þ
__s32 rat_is_favor_file(HRAT hrat, char* filename)
{
	__rat_list_t* pRatList;
	ScanInfo_t *pScanInfo;
	__u32 cnt;
    __s32 bok;
    ES_FILE* fp;
    __s32 result;
    
    result = 0;
    
	pRatList = (__rat_list_t*)hrat;
	if (0 == pRatList || 0 == pRatList->hscan)
	{
		return result;
	}

    if(NULL == filename)
    {
        DebugPrintf("invalid para...\n");
        return result;
    }

    
    bok = EPDK_OK;    
    fp = NULL;  
	
	RAT_ENTER_CRITICAL_SECTION;
	pScanInfo = (ScanInfo_t *)(pRatList->hscan);

    if(RAT_MEDIA_TYPE_FAVOR_AUDIO != pRatList->media_type
        &&RAT_MEDIA_TYPE_FAVOR_VIDEO != pRatList->media_type
        &&RAT_MEDIA_TYPE_FAVOR_PIC != pRatList->media_type
        &&RAT_MEDIA_TYPE_FAVOR_EBOOK != pRatList->media_type)
    {
        DebugPrintf("invalid media type...\n");
        RAT_LEAVE_CRITICAL_SECTION;
        return result;
    }

    do{//´¦ÀíÊÕ²ØÀàÐÍÉ¨Ãè
        __s32 ret;
        __s32 cnt;
        __s32 val;
        __s32 num;
        __s32 i;
        __s32 fsize;
                
        char str_db_name[32]={0};
        char str_db_path[1024]={0};
        char str_file_item[1024]={0};

        cnt = 0;              
        
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
            bok = EPDK_FAIL;
            break;
        }

        eLIBs_sprintf(str_db_path, "%c:\\%s%s", filename[0],
            "favor_db\\", str_db_name);
		
		//DebugPrintf("filename[0]=%s\n", filename[0]);
		DebugPrintf("str_db_name=%s\n", str_db_name);
		DebugPrintf("str_db_path=%s\n", str_db_path);

        fp = eLIBs_fopen(str_db_path, "rb");
        if(NULL == fp)
        {
            bok = EPDK_FAIL;
            DebugPrintf("file does not exist,str_db_path=%s\n", str_db_path);
            goto end;
        }

        ret = eLIBs_fread(&val, 1, 4, fp);
        if(ret != 0 && ret < 4)
        {
            bok = EPDK_FAIL;
            DebugPrintf("eLIBs_fread fail...\n");
            goto end;
        }

        if(0 == ret)//³õÊ¼´´½¨Êý¾ÝÎÄ¼þ
        {
            char tmp[16]={0};
            DebugPrintf("item count is 0, begin create db...\n");
            val = 0;
            num = 0;

            eLIBs_fseek(fp, 0, SEEK_SET);
            eLIBs_fwrite(tmp, 1, 16, fp);
            goto save_item;
        }

        fsize = val;
        DebugPrintf("fsize=%d\n", fsize);

        eLIBs_fseek(fp, 4, SEEK_SET);
        ret = eLIBs_fread(&val, 1, 4, fp);
        if(4 != ret)
        {
            DebugPrintf("eLIBs_fread fail...\n");
            bok = EPDK_FAIL;
            goto end;
        }

        num = val;

		DebugPrintf("num=%d\n", num);

        eLIBs_fseek(fp, 0, SEEK_END);
        ret = eLIBs_ftell(fp);
        if(ret < num*1024+16
            ||ret < fsize)
        {
            bok = EPDK_FAIL;
            DebugPrintf("eLIBs_fread fail...\n");
            goto del_db;
        }

        eLIBs_fseek(fp, 16, SEEK_SET);
        
        
        for(i = 0 ; i < num ; i++)
        {
            ret = eLIBs_fread(str_file_item, 1, 1024, fp);
            if(1024 != ret)
            {
                bok = EPDK_FAIL;
                DebugPrintf("eLIBs_fread fail...\n");
                goto end;
            }

            str_file_item[0] = filename[0];//ÐÞ¸ÄÂ·¾¶ÅÌ·û
            ret = eLIBs_stricmp(str_file_item, filename);
            if(0 == ret)
            {
                result = 1;
                DebugPrintf("identify str found...\n");
                goto end;
            }            
        }        

        save_item:;
                
              
        
        end:;
        if(fp)
        {
            eLIBs_fclose(fp);
            fp = NULL;
        }
        break;

        del_db:;
        if(fp)
        {
            eLIBs_fclose(fp);
            fp = NULL;
        }

        eLIBs_remove(str_db_path);        

        break;
        
    }while(0);

    RAT_LEAVE_CRITICAL_SECTION;
    
    return result;       
}

//0:²»ÊÇÊÕ²ØÎÄ¼þ£¬1:ÊÇÊÕ²ØÎÄ¼
__s32 rat_get_Fav_item_info_by_index(HRAT hRat, __s32 index, char *filename, char *path )
{
	__rat_list_t* pRatList;
	ScanInfo_t *pScanInfo;
	__u32 cnt;
    __s32 bok;
    ES_FILE* fp;
    __s32 result;
    
       DebugPrintf("index=%d\n", index);
    __wrn("index=%d\n", index);

	result = 0;
    
	if (NULL == hRat)
	{
        __here__;
		DebugPrintf("hRat is NULL\n");
		return EPDK_FAIL;
	}		

	pRatList = (__rat_list_t*)hRat;
	if (0 == pRatList || 0 == pRatList->hscan)
	{
		return result;
	}

    
    bok = EPDK_OK;    
    fp = NULL;  
	
	RAT_ENTER_CRITICAL_SECTION;
	pScanInfo = (ScanInfo_t *)(pRatList->hscan);

    if(RAT_MEDIA_TYPE_FAVOR_AUDIO != pRatList->media_type
        &&RAT_MEDIA_TYPE_FAVOR_VIDEO != pRatList->media_type
        &&RAT_MEDIA_TYPE_FAVOR_PIC != pRatList->media_type
        &&RAT_MEDIA_TYPE_FAVOR_EBOOK != pRatList->media_type)
    {
        DebugPrintf("invalid media type...\n");
        RAT_LEAVE_CRITICAL_SECTION;
        return result;
    }

    do{//´¦ÀíÊÕ²ØÀàÐÍÉ¨Ãè
        __s32 ret;
        __s32 cnt;
        __s32 val;
        __s32 num;
        __s32 i;
        __s32 fsize;
                
        char str_db_name[32]={0};
        char str_db_path[1024]={0};
        char str_file_item[1024]={0};

        cnt = 0;              
        
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
            bok = EPDK_FAIL;
            break;
        }

        eLIBs_sprintf(str_db_path, "%c:\\%s%s", path[0],
            "favor_db\\", str_db_name);
		
		//DebugPrintf("filename[0]=%s\n", filename[0]);
		DebugPrintf("str_db_name=%s\n", str_db_name);
		DebugPrintf("str_db_path=%s\n", str_db_path);

        fp = eLIBs_fopen(str_db_path, "rb");
        if(NULL == fp)
        {
            bok = EPDK_FAIL;
            DebugPrintf("file does not exist,str_db_path=%s\n", str_db_path);
            goto end;
        }

        ret = eLIBs_fread(&val, 1, 4, fp);
        if(ret != 0 && ret < 4)
        {
            bok = EPDK_FAIL;
            DebugPrintf("eLIBs_fread fail...\n");
            goto end;
        }

        if(0 == ret)//³õÊ¼´´½¨Êý¾ÝÎÄ¼þ
        {
            char tmp[16]={0};
            DebugPrintf("item count is 0, begin create db...\n");
            val = 0;
            num = 0;

            eLIBs_fseek(fp, 0, SEEK_SET);
            eLIBs_fwrite(tmp, 1, 16, fp);
            goto save_item;
        }

        fsize = val;
        DebugPrintf("fsize=%d\n", fsize);

        eLIBs_fseek(fp, 4, SEEK_SET);
        ret = eLIBs_fread(&val, 1, 4, fp);
        if(4 != ret)
        {
            DebugPrintf("eLIBs_fread fail...\n");
            bok = EPDK_FAIL;
            goto end;
        }

        num = val;

		DebugPrintf("num=%d\n", num);

        eLIBs_fseek(fp, 0, SEEK_END);
        ret = eLIBs_ftell(fp);
        if(ret < num*1024+16
            ||ret < fsize)
        {
            bok = EPDK_FAIL;
            DebugPrintf("eLIBs_fread fail...\n");
            goto del_db;
        }

        eLIBs_fseek(fp, 16, SEEK_SET);
        
        
        for(i = 0 ; i <= index; i++)
        {
            ret = eLIBs_fread(str_file_item, 1, 1024, fp);
            if(1024 != ret)
            {
                bok = EPDK_FAIL;
                DebugPrintf("eLIBs_fread fail...\n");
                goto end;
            }
	}
            str_file_item[0] = path[0];//ÐÞ¸ÄÂ·¾¶ÅÌ·û
           
                DebugPrintf("str_file_item =%s..\n",str_file_item);
           filename = str_file_item;
		   // ret = eLIBs_strcpy(filename,str_file_item);
		   DebugPrintf("filename =%s..\n",filename);
             eLIBs_strcpy(filename,str_file_item);
              DebugPrintf("filename =%s..\n",filename);
                           result = 1;
           //     DebugPrintf("identify str found...\n");
                goto end;
                       
                

        save_item:;
                
              
        
        end:;
        if(fp)
        {
            eLIBs_fclose(fp);
            fp = NULL;
        }
        break;

        del_db:;
        if(fp)
        {
            eLIBs_fclose(fp);
            fp = NULL;
        }

        //eLIBs_remove(str_db_path);        

        break;
        
    }while(0);

    RAT_LEAVE_CRITICAL_SECTION;
    
    return result;       
}



