//#include "digital_db.h"

/*
4 uipara
3 init_res   uinit_res
2 win_cb
1 create main win
*/
#if 0
void *digital_db_scene_create(H_WIN h_parent, __s32 win_id, __u32 style,
								__u32 timeout)
{
	digital_wnd_t *digital_win_para;

	__inf("****************************************************************\n");
	__inf("********  enter dialog app  **************\n");
	__inf("****************************************************************\n");	

	BallocType(digital_win_para, digital_wnd_t);
	if(wnd_para == NULL)
	{
		return NULL;
	}
	else
	{
		__msg("app_dialog wnd para ============================%x\n", digital_win_para);
	}
	eLIBs_memset(digital_win_para, 0, sizeof(dialog_wnd_t));

	digital_win_para->parent  = h_parent;
	digital_win_para->ID      = win_id;
	digital_win_para->style   = style;
	digital_win_para->timeout = timeout;
	app_dialog_data_init(digital_win_para, bmp_id, str_id, NULL);

	app_dialog_layer_create(digital_win_para->hlyr, &digital_win_para->ui.rt);

	if (digital_win_para->hlyr == NULL)
	{//TODO:release mem
		__err("app_dialog_layer_create error(%d, %d)\n", digital_win_para->ui.rt.width, digital_win_para->ui.rt.height);
	}

	digital_win_para->hwnd = app_dialog_create_frmwin(digital_win_para);

	GUI_WinSetFocusChild(digital_win_para->hwnd);

	return digital_win_para->hwnd;
	

#if 0
	dialog_wnd_t *wnd_para;

	__inf("****************************************************************\n");
	__inf("********  enter dialog app  **************\n");
	__inf("****************************************************************\n");
	mute_flag=0;

	BallocType(wnd_para, dialog_wnd_t);
	if (wnd_para == NULL)
	{
		return NULL;
	}
	else
	{
		__msg("app_dialog wnd para ============== %x\n", wnd_para);
	}
	eLIBs_memset(wnd_para, 0, sizeof(dialog_wnd_t));

	wnd_para->parent  = h_parent;
	wnd_para->ID      = win_id;
	wnd_para->style   = style;
	wnd_para->timeout = timeout;
	app_dialog_data_init(wnd_para, bmp_id, str_id, NULL);

	app_dialog_layer_create(wnd_para->hlyr, &wnd_para->ui.rt);

	if (wnd_para->hlyr == NULL)
	{//TODO:release mem
		__err("app_dialog_layer_create error(%d, %d)\n", wnd_para->ui.rt.width, wnd_para->ui.rt.height);
	}

	wnd_para->hwnd = app_dialog_create_frmwin(wnd_para);

	GUI_WinSetFocusChild(wnd_para->hwnd);

	return wnd_para->hwnd;
#endif	
}
#endif


