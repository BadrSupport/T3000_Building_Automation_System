// TstatAQ.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "T3000.h"
#include "TstatAQ.h"
#include "MainFrm.h"

// CTstatAQ
#define     WM_TSTAT_AQ_THREAD_READ                     WM_USER + 502
HANDLE h_tstat_aq_thread = NULL;
IMPLEMENT_DYNCREATE(CTstatAQ, CFormView)

CTstatAQ::CTstatAQ()
	: CFormView(IDD_DIALOG_TSTAT_AQ)
{

}

CTstatAQ::~CTstatAQ()
{
}

void CTstatAQ::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTstatAQ, CFormView)
    ON_MESSAGE(WM_TSTAT_AQ_THREAD_READ, UpdateUI)
    ON_EN_KILLFOCUS(IDC_EDIT_CO2_ON_TIME, &CTstatAQ::OnEnKillfocusEditCo2OnTime)
    ON_EN_KILLFOCUS(IDC_EDIT_CO2_OFF_TIME, &CTstatAQ::OnEnKillfocusEditCo2OffTime)
    ON_EN_KILLFOCUS(IDC_EDIT_PM_ON_TIME, &CTstatAQ::OnEnKillfocusEditPmOnTime)
    ON_EN_KILLFOCUS(IDC_EDIT_PM_OFF_TIME, &CTstatAQ::OnEnKillfocusEditPmOffTime)
END_MESSAGE_MAP()


// CTstatAQ ���

#ifdef _DEBUG
void CTstatAQ::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CTstatAQ::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG
CString AQ_image_fordor;
void CTstatAQ::Fresh()
{
    CStatic* pWnd = (CStatic*)GetDlgItem(IDC_STATIC_AQ_TEMPERATURE); // �õ� Picture Control ��� ;
    CString icon_temperature;
    
   
    CString ApplicationFolder;
    GetModuleFileName(NULL, ApplicationFolder.GetBuffer(MAX_PATH), MAX_PATH);
    PathRemoveFileSpec(ApplicationFolder.GetBuffer(MAX_PATH));
    ApplicationFolder.ReleaseBuffer();
    AQ_image_fordor = ApplicationFolder + _T("\\ResourceFile");
    icon_temperature = AQ_image_fordor + _T("\\temperature.bmp");
    HBITMAP bitmap;
    bitmap = (HBITMAP)LoadImage(AfxGetInstanceHandle(), icon_temperature, IMAGE_BITMAP, 40, 120, LR_LOADFROMFILE);

    CStatic *p = (CStatic *)GetDlgItem(IDC_STATIC_AQ_TEMPERATURE);
    //���þ�̬�ؼ����ڷ��Ϊλͼ������ʾ  
    p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
    //��ͼƬ���õ�Picture�ؼ���  
    p->SetBitmap(bitmap);





    CMainFrame* pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
    pFrame->SetWindowTextW(cs_special_name + CurrentT3000Version);
    if (h_tstat_aq_thread == NULL)
        h_tstat_aq_thread = CreateThread(NULL, NULL, Update_TstatAQ_Thread, this, NULL, NULL);
    UpdateUI();


    CString sound_full_path;
    CStatic* pWnd_sound_pic = (CStatic*)GetDlgItem(IDC_STATIC_SOUND); // �õ� Picture Control ��� ;
    if(product_register_value[TSTAT_AQ_SOUND] <= 10)
        sound_full_path = ApplicationFolder + _T("\\ResourceFile\\Icon\\sound_0.ico");
    else if (product_register_value[TSTAT_AQ_SOUND] <= 30)
        sound_full_path = ApplicationFolder + _T("\\ResourceFile\\Icon\\sound_1.ico");
    else if (product_register_value[TSTAT_AQ_SOUND] <= 50)
        sound_full_path = ApplicationFolder + _T("\\ResourceFile\\Icon\\sound_2.ico");
    else 
        sound_full_path = ApplicationFolder + _T("\\ResourceFile\\Icon\\sound_3.ico");
    Bitmap icon_bitmap(sound_full_path);
    HICON m_hIcon = ExtractIcon(AfxGetInstanceHandle(), sound_full_path,0);
    pWnd_sound_pic->ModifyStyle(0, SS_ICON | SS_CENTERIMAGE);
    pWnd_sound_pic->SetIcon(m_hIcon);

   

}

void CTstatAQ::UpdateUI()
{
    CString cs_temp;
    cs_temp.Format(_T("%.1f"), ((float)product_register_value[TSTAT_AQ_TEMPERATURE]) / 10);

    CString cs_hum;
    cs_hum.Format(_T("%.1f"), ((float)product_register_value[TSTAT_AQ_HUM]) / 10);

    CString cs_ppm;
    cs_ppm.Format(_T("%d"), product_register_value[TSTAT_AQ_CO2]);

    CString cs_VOC;
    cs_VOC.Format(_T("%d"), product_register_value[TSTAT_AQ_VOC]);

    CString cs_light;
    cs_light.Format(_T("%u"), product_register_value[TSTAT_AQ_LIGHT]);

    CString cs_sound;
    cs_sound.Format(_T("%u"), product_register_value[TSTAT_AQ_SOUND]);

    CString cs_co2_on;
    CString cs_co2_off;
    CString cs_pm_on;
    CString cs_pm_off;
    cs_co2_on.Format(_T("%u"), product_register_value[TATAT_AQ_CO2_ON]);
    cs_co2_off.Format(_T("%u"), product_register_value[TATAT_AQ_CO2_OFF]);
    cs_pm_on.Format(_T("%u"), product_register_value[TATAT_AQ_PM_ON]);
    cs_pm_off.Format(_T("%u"), product_register_value[TATAT_AQ_PM_OFF]);

    GetDlgItem(IDC_STATIC_TEMPERATURE_VALUE)->SetWindowTextW(cs_temp);
    GetDlgItem(IDC_STATIC_HUM_VALUE)->SetWindowTextW(cs_hum);
    GetDlgItem(IDC_STATIC_CO2_VALUE)->SetWindowTextW(cs_ppm);
    GetDlgItem(IDC_STATIC_VOC_VALUE)->SetWindowTextW(cs_VOC);
    GetDlgItem(IDC_STATIC_LIGHT_VALUE)->SetWindowTextW(cs_light);
    GetDlgItem(IDC_STATIC_SOUND_VALUE)->SetWindowTextW(cs_sound);

    GetDlgItem(IDC_EDIT_CO2_ON_TIME)->SetWindowTextW(cs_co2_on);
    GetDlgItem(IDC_EDIT_CO2_OFF_TIME)->SetWindowTextW(cs_co2_off);
    GetDlgItem(IDC_EDIT_PM_ON_TIME)->SetWindowTextW(cs_pm_on);
    GetDlgItem(IDC_EDIT_PM_OFF_TIME)->SetWindowTextW(cs_pm_off);

    CString cs_weight_pm1;
    if (product_register_value[TSTAT_AQ_WEIGHT_1] != 0)
        cs_weight_pm1.Format(_T("%u"), product_register_value[TSTAT_AQ_WEIGHT_1]);
    else
        cs_weight_pm1 = _T("-");
    CString cs_weight_pm2_5;
    if(product_register_value[TSTAT_AQ_WEIGHT_2_5] != 0)
        cs_weight_pm2_5.Format(_T("%u"), product_register_value[TSTAT_AQ_WEIGHT_2_5]);
    else
        cs_weight_pm2_5 = _T("-");
    CString cs_weight_pm4;
    if(product_register_value[TSTAT_AQ_WEIGHT_4] != 0)
        cs_weight_pm4.Format(_T("%u"), product_register_value[TSTAT_AQ_WEIGHT_4]);
    else
        cs_weight_pm4 = _T("-");
    CString cs_weight_pm10;
    if(product_register_value[TSTAT_AQ_WEIGHT_10] != 0)
        cs_weight_pm10.Format(_T("%u"), product_register_value[TSTAT_AQ_WEIGHT_10]);
    else
        cs_weight_pm10 = _T("-");

    CString cs_weight_total;
    cs_weight_total.Format(_T("%u"), product_register_value[TSTAT_AQ_WEIGHT_1] +
        product_register_value[TSTAT_AQ_WEIGHT_2_5] +
        product_register_value[TSTAT_AQ_WEIGHT_4] +
        product_register_value[TSTAT_AQ_WEIGHT_10]);

    GetDlgItem(IDC_STATIC_WEIGHT_PM1_0)->SetWindowTextW(cs_weight_pm1);
    GetDlgItem(IDC_STATIC_WEIGHT_PM2_5)->SetWindowTextW(cs_weight_pm2_5);
    GetDlgItem(IDC_STATIC_WEIGHT_PM4_0)->SetWindowTextW(cs_weight_pm4);
    GetDlgItem(IDC_STATIC_WEIGHT_PM10)->SetWindowTextW(cs_weight_pm10);
    GetDlgItem(IDC_STATIC_WEIGHT_TOTAL)->SetWindowTextW(cs_weight_total);

    CString cs_index_pm1;
    if(product_register_value[TSTAT_AQ_INDEX_1] != 0)
        cs_index_pm1.Format(_T("%u"), product_register_value[TSTAT_AQ_INDEX_1]);
    else
        cs_index_pm1 = _T("-");
    CString cs_index_pm2_5;
    if(product_register_value[TSTAT_AQ_INDEX_2_5] != 0)
        cs_index_pm2_5.Format(_T("%u"), product_register_value[TSTAT_AQ_INDEX_2_5]);
    else
        cs_index_pm2_5 = _T("-");
    CString cs_index_pm4;
    if(product_register_value[TSTAT_AQ_INDEX_4] != 0)
        cs_index_pm4.Format(_T("%u"), product_register_value[TSTAT_AQ_INDEX_4]);
    else
        cs_index_pm4 = _T("-");
    CString cs_index_pm10;
    if(product_register_value[TSTAT_AQ_INDEX_10] != 0)
        cs_index_pm10.Format(_T("%u"), product_register_value[TSTAT_AQ_INDEX_10]);
    else
        cs_index_pm10 = _T("-");

    CString cs_index_total;
    cs_index_total.Format(_T("%u"), product_register_value[TSTAT_AQ_INDEX_1] +
        product_register_value[TSTAT_AQ_INDEX_2_5] +
        product_register_value[TSTAT_AQ_INDEX_4] +
        product_register_value[TSTAT_AQ_INDEX_10]);

    GetDlgItem(IDC_STATIC_INDEX_PM1_0)->SetWindowTextW(cs_index_pm1);
    GetDlgItem(IDC_STATIC_INDEX_PM2_5)->SetWindowTextW(cs_index_pm2_5);
    GetDlgItem(IDC_STATIC_INDEX_PM4_0)->SetWindowTextW(cs_index_pm4);
    GetDlgItem(IDC_STATIC_INDEX_PM10)->SetWindowTextW(cs_index_pm10);
    GetDlgItem(IDC_STATIC_INDEX_TOTAL)->SetWindowTextW(cs_index_total);
}
// CTstatAQ ��Ϣ�������


void CTstatAQ::OnInitialUpdate()
{
    CFormView::OnInitialUpdate();
    // TODO: �ڴ����ר�ô����/����û���
}

DWORD WINAPI Update_TstatAQ_Thread(LPVOID lPvoid)
{
    CTstatAQ * mparent = (CTstatAQ *)lPvoid;
    while (mparent->IsWindowVisible())
    {
         Read_Multi(g_tstat_id, &product_register_value[100],100, 100, 5);
         Read_Multi(g_tstat_id, &product_register_value[700], 700, 100, 5);
         Read_Multi(g_tstat_id, &product_register_value[950], 950, 100, 5);
        //for (int i = 0; i < 5; i++)
        //{
        //    int itemp = 0;
        //    itemp = Read_Multi(g_tstat_id, &product_register_value[i * 100], i * 100, 100, 5);
        //}
        PostMessage(mparent->m_hWnd, WM_TSTAT_AQ_THREAD_READ, NULL, NULL);
        Sleep(7000);
    }


    h_tstat_aq_thread = NULL;
    return 1;
}

LRESULT CTstatAQ::UpdateUI(WPARAM wParam, LPARAM lParam)
{
    Fresh();
    return 0;
}


void CTstatAQ::OnEnKillfocusEditCo2OnTime()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������

    CString strText;
    GetDlgItem(IDC_EDIT_CO2_ON_TIME)->GetWindowText(strText);
    int nValue = (int)(_wtoi(strText));


    if (product_register_value[TATAT_AQ_CO2_ON] == nValue)	//Add this to judge weather this value need to change.
        return;


    Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, TATAT_AQ_CO2_ON, nValue,
        product_register_value[TATAT_AQ_CO2_ON], this->m_hWnd, IDC_EDIT_DTERM, _T("CO2 On Time"));
}


void CTstatAQ::OnEnKillfocusEditCo2OffTime()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    CString strText;
    GetDlgItem(IDC_EDIT_CO2_OFF_TIME)->GetWindowText(strText);
    int nValue = (int)(_wtoi(strText));


    if (product_register_value[TATAT_AQ_CO2_OFF] == nValue)	//Add this to judge weather this value need to change.
        return;


    Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, TATAT_AQ_CO2_OFF, nValue,
        product_register_value[TATAT_AQ_CO2_OFF], this->m_hWnd, IDC_EDIT_DTERM, _T("CO2 Off Time"));
}


void CTstatAQ::OnEnKillfocusEditPmOnTime()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    CString strText;
    GetDlgItem(IDC_EDIT_PM_ON_TIME)->GetWindowText(strText);
    int nValue = (int)(_wtoi(strText));


    if (product_register_value[TATAT_AQ_PM_ON] == nValue)	//Add this to judge weather this value need to change.
        return;


    Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, TATAT_AQ_PM_ON, nValue,
        product_register_value[TATAT_AQ_PM_ON], this->m_hWnd, IDC_EDIT_DTERM, _T("PM On Time"));
}


void CTstatAQ::OnEnKillfocusEditPmOffTime()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    CString strText;
    GetDlgItem(IDC_EDIT_PM_OFF_TIME)->GetWindowText(strText);
    int nValue = (int)(_wtoi(strText));


    if (product_register_value[TATAT_AQ_PM_OFF] == nValue)	//Add this to judge weather this value need to change.
        return;


    Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, TATAT_AQ_PM_OFF, nValue,
        product_register_value[TATAT_AQ_PM_OFF], this->m_hWnd, IDC_EDIT_DTERM, _T("PM Off Time"));
}
