
// 18.TetrisView.cpp : CMy18TetrisView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "18.Tetris.h"
#endif

#include "18.TetrisDoc.h"
#include "18.TetrisView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const int speed = 1000;
const int kindNum = 7;
const int RussiaData[kindNum][4][2] = { { { -1,-1 },{ 1,-1 },{ 1,1 },{ -1,1 } },  //����
{ { -3,0 },{ -1,0 },{ 1,0 },{ 3,0 } },  // ����
{ { -2,0 },{ 0,0 },{ 0,-2 },{ 2,0 } },  // ���� 
{ { -2,0 },{ -2,2 },{ 0,2 },{ 2,2 } },  // ���L
{ { -2,2 },{ 0,2 },{ 2,2 },{ 2,0 } },  // ��ķ�L
{ { -2,-1 },{ 0,-1 },{ 0,1 },{ 2,1 } },  // ֮��
{ { -2,1 },{ 0,1 },{ 0,-1 },{ 2,-1 } }  // ��֮��						 
}; //��һ��������row,�����Ͻ����ң��ڶ���������col,�����½�����
// CMy18TetrisView

IMPLEMENT_DYNCREATE(CMy18TetrisView, CView)

BEGIN_MESSAGE_MAP(CMy18TetrisView, CView)
	/*-------------�Լ��������Ϣӳ��-----------*/
	ON_WM_TIMER() //����timer��Ϣ
	ON_WM_CREATE()
	ON_WM_KEYDOWN() //keydown���� ����:����û��Ƿ��˼�����ָ���ļ�
	/*-------------�Լ��������Ϣӳ��-----------*/
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMy18TetrisView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

void CMy18TetrisView::start() { //������ͼ��
	rowPos = rowCount + 2;
	colPos = 1;
	kind = rand() % 7; //���ȡ��״
	for (int i = 0; i<4; ++i)
		for (int j = 0; j<2; ++j)
			shift[i][j] = RussiaData[kind][i][j]; //��ֵ����ɸ�����״
}// ��ʼ���������

bool CMy18TetrisView::GoDown() { //���·�������¡���ʱ��,���߼�ʱ������ʱ
	for (int w = 0; w<4; ++w) {
		int i = (shift[w][0] + rowPos) / 2;
		int j = (shift[w][1] + colPos) / 2 + 1; //����������һ�������ǲ����Ѿ��ǵ�����
		if (GameMap[i][j] == 1) return true; //��ط���true
	}
	colPos += 2;
	Invalidate();
	return false;
}
void CMy18TetrisView::trans() { //�������ת��Ϊ��ͼ
	for (int w = 0; w<4; ++w) {
		int i = (shift[w][0] + rowPos) / 2;
		int j = (shift[w][1] + colPos) / 2;
		GameMap[i][j] = 1;
	}
}
bool CMy18TetrisView::IsOver() { //�жϵ��ﶥ�ˣ���Ϸ����
	for (int i = 1; i <= rowCount; ++i)
		if (GameMap[i][0] == 1) return true;
	return false;
}

void CMy18TetrisView::GoLeft() { //���·�������󡱵�ʱ��
	for (int w = 0; w<4; ++w) {
		int i = (shift[w][0] + rowPos) / 2 - 1;
		int j = (shift[w][1] + colPos) / 2;
		if (GameMap[i][j] == 1) return; //����߽�Ļ�ֱ�ӷ���
	}
	rowPos -= 2;
	Invalidate();
}

void CMy18TetrisView::GoRight() {  //���·�������ҡ���ʱ��
	for (int w = 0; w<4; ++w) {
		int i = (shift[w][0] + rowPos) / 2 + 1;
		int j = (shift[w][1] + colPos) / 2;
		if (GameMap[i][j] == 1) return; //���ұ߽�Ļ�ֱ�ӷ���
	}
	rowPos += 2;
	Invalidate();
}

void CMy18TetrisView::ShapeSwitch() { //���·�������ϡ���ʱ�򣬸ı���״
	int w;
	for (w = 0; w<4; ++w) {
		int i = (shift[w][1] + rowPos) / 2;
		int j = (-shift[w][0] + colPos) / 2;
		if (GameMap[i][j] == 1) return; //����ס�ˣ��޷�������
	}
	for (w = 0; w<4; ++w) { //��ʱ����ת90��,ֻ����
		shift[w][1] = shift[w][1] + shift[w][0];
		shift[w][0] = shift[w][1] - shift[w][0];
		shift[w][1] = -(shift[w][1] - shift[w][0]);
	}
	Invalidate();
}
void CMy18TetrisView::LineDelete() { //����һ�У��Ѹ����Ϸ���ȫ�����鼯��������
	int cnt;
	int i = colCount;
	while (i) {
		cnt = 0;
		for (int j = 1; j <= rowCount; ++j)
			if (GameMap[j][i] == 1) ++cnt;
		if (cnt == rowCount) {
			for (int w = i; w > 0; w--)
				for (int j = 1; j <= rowCount; ++j)
					GameMap[j][w] = GameMap[j][w - 1];  //���������ƶ�
		}
		else i--;
	}
	Invalidate();
}
// CMy18TetrisView ����/����

CMy18TetrisView::CMy18TetrisView()
{
	// TODO: �ڴ˴���ӹ������
	rowCount = 12;//һ��13������
	elen = 15;
	colCount = 20; //��Ļ��20������
	block.LoadBitmap(IDB_BITMAP1);//CBitmap�����������ͼƬ
	srand(time(0)); //�����ɶ���أ���ʼ��������ӣ�

	memset(GameMap, 1, sizeof(GameMap));
	for (int i = 1; i <= rowCount; ++i)
		for (int j = 0; j <= colCount; ++j)
			GameMap[i][j] = 0;
	start();
}

CMy18TetrisView::~CMy18TetrisView()
{
}

BOOL CMy18TetrisView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CMy18TetrisView ����

void CMy18TetrisView::OnDraw(CDC* pDC)
{
	CMy18TetrisDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
	//CBrush background;
	//background.CreateSolidBrush(RGB(233, 233, 233));

	CBrush background;
	background.CreateSolidBrush(RGB(233, 233, 233));

	CDC Dc;
	if (Dc.CreateCompatibleDC(pDC) == FALSE)
		AfxMessageBox(_T("Can't create DC"));
	Dc.SelectObject(block);

	for (int i = 1; i <= rowCount; ++i)
		for (int j = 1; j <= colCount; ++j) {
			if (GameMap[i][j] == 0) {
				CRect myrect(i*elen * 2, j*elen * 2, (i + 1)*elen * 2, (j + 1)*elen * 2); //���ε��ĸ�����
				pDC->FillRect(myrect, &background);
			}
			/*
			x��Ŀ�������������Ͻ�x������㡣
			y��Ŀ�������������Ͻ�y������㡣
			nWidth����Ŀ���豸�л���λͼ�Ŀ�ȡ�
			nHight����Ŀ���豸�л���λͼ�ĸ߶ȡ�
			pSrcDC��Դ�豸�����Ķ���ָ�롣
			xSrc��Դ�豸�����ĵ����x�����꣬�����Ӹ���㸴��λͼ��Ŀ���豸��
			ySrc��Դ�豸�����ĵ����y�����꣬�����Ӹ���㸴��λͼ��Ŀ���豸��
			dwRop����դ��������
			*/
			else  pDC->BitBlt(i*elen * 2, j*elen * 2, (i + 1)*elen * 2, (j + 1)*elen * 2, &Dc, 0, 0, SRCCOPY);
		}

	for (int w = 0; w<4; ++w) {
		int i = (shift[w][0] + rowPos) / 2;
		int j = (shift[w][1] + colPos) / 2;
		//�ú�����ָ����Դ�豸���������е����ؽ���λ�飨bit_block��ת�����Դ��͵�Ŀ���豸����
		//src_copy ��Դ��������ֱ�ӿ�����Ŀ���������
		if (i > 0 && j > 0) pDC->BitBlt(i*elen * 2, j*elen * 2, (i + 1)*elen * 2, (j + 1)*elen * 2, &Dc, 0, 0, SRCCOPY);
	}
}


// CMy18TetrisView ��ӡ


void CMy18TetrisView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMy18TetrisView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CMy18TetrisView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CMy18TetrisView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}

void CMy18TetrisView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMy18TetrisView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMy18TetrisView ���

#ifdef _DEBUG
void CMy18TetrisView::AssertValid() const
{
	CView::AssertValid();
}

void CMy18TetrisView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMy18TetrisDoc* CMy18TetrisView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMy18TetrisDoc)));
	return (CMy18TetrisDoc*)m_pDocument;
}
#endif //_DEBUG


// CMy18TetrisView ��Ϣ�������
void CMy18TetrisView::OnTimer(UINT nIDEvent) //��ʱ������
{
	// TODO: Add your message handler code here and/or call default
	if (GoDown() == true) {
		trans(); //��ʾ���������
		if (IsOver()) {
			memset(GameMap, 1, sizeof(GameMap)); //��Ϸ����
		}
		else
		{
			LineDelete();//������û��������ɾ������
			start(); //����������ͼ��
		}
		Invalidate();
	}
	CView::OnTimer(nIDEvent);
}

int CMy18TetrisView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: Add your specialized creation code here
	SetTimer(1, speed, NULL);
	return 0;
}
void CMy18TetrisView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) //�������µ�ʱ�򡣴�������Ϣ
{
	// TODO: Add your message handler code here and/or call default
	switch (nChar)
	{
	case VK_NUMPAD0://С���ּ���0
		start();
		break;
	case VK_DOWN:  //visual keyboard
		GoDown();
		break;
	case VK_LEFT:
		GoLeft();
		break;
	case VK_RIGHT:
		GoRight();
		break;
	case VK_UP:
		ShapeSwitch();
	default: break;
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}