#pragma once


// CBacnetSettingParamter �Ի���

class CBacnetSettingParamter : public CDialogEx
{
	DECLARE_DYNAMIC(CBacnetSettingParamter)

public:
	CBacnetSettingParamter(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBacnetSettingParamter();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_BACNET_SETTING_LCD_PARAMETER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    void InitialUI();
    void LCD_CUS_MODE(bool cus_mode);
    afx_msg void OnBnClickedRadioCusDef();
    afx_msg void OnBnClickedRadioLcdDefault();
    afx_msg void OnCbnSelchangeComboType();
    afx_msg void OnCbnSelchangeComboNumber();
};
