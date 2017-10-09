
// 18.TetrisView.h : CMy18TetrisView ��Ľӿ�
//

#pragma once


class CMy18TetrisView : public CView
{
protected: // �������л�����
	CMy18TetrisView();
	DECLARE_DYNCREATE(CMy18TetrisView)

// ����
public:
	CMy18TetrisDoc* GetDocument() const;

// ����
public:
/*---------------------�Լ���������Ա-----------------*/
public:
	void start();   // ��ʼ���������������ͼ��
	void trans(); //  �������ת��Ϊ��ͼ
	void LineDelete();   // ��ȥ���Ƿ������ 
	void ShapeSwitch(); // ���� ��ת
	bool GoDown();     // ���� ��ط���true
	void GoLeft();    // ����
	void GoRight();  // ����
	bool IsOver();  // �Ƿ���Ϸʧ����
private:
	int rowCount;   // ������
	int colCount;  // �������
	int elen;     // ��񳤶�
	bool GameMap[30][30];
	int rowPos, colPos;         // ������������
	int shift[4][2];          // ������ĵ�ƫ����  ���Ϊ��λ
	int kind;                // ����
	CBitmap block;         //����λͼ
 /*---------------------�Լ���������Ա-----------------*/
// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CMy18TetrisView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
/*-----------------�Լ��������Ϣӳ�亯��-----------------*/
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags); //����������������������������Ϣ���ݰ�����
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
/*-----------------�Լ��������Ϣӳ�亯��-----------------*/
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // 18.TetrisView.cpp �еĵ��԰汾
inline CMy18TetrisDoc* CMy18TetrisView::GetDocument() const
   { return reinterpret_cast<CMy18TetrisDoc*>(m_pDocument); }
#endif

