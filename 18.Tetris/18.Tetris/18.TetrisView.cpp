
// 18.TetrisView.cpp : CMy18TetrisView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
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
const int RussiaData[kindNum][4][2] = { { { -1,-1 },{ 1,-1 },{ 1,1 },{ -1,1 } },  //方块
{ { -3,0 },{ -1,0 },{ 1,0 },{ 3,0 } },  // 长条
{ { -2,0 },{ 0,0 },{ 0,-2 },{ 2,0 } },  // 土型 
{ { -2,0 },{ -2,2 },{ 0,2 },{ 2,2 } },  // 横的L
{ { -2,2 },{ 0,2 },{ 2,2 },{ 2,0 } },  // 横的反L
{ { -2,-1 },{ 0,-1 },{ 0,1 },{ 2,1 } },  // 之形
{ { -2,1 },{ 0,1 },{ 0,-1 },{ 2,-1 } }  // 反之形						 
}; //第一个坐标是row,从左上角往右，第二个坐标是col,从左下角往下
// CMy18TetrisView

IMPLEMENT_DYNCREATE(CMy18TetrisView, CView)

BEGIN_MESSAGE_MAP(CMy18TetrisView, CView)
	/*-------------自己加入的消息映射-----------*/
	ON_WM_TIMER() //定义timer消息
	ON_WM_CREATE()
	ON_WM_KEYDOWN() //keydown函数 功能:检查用户是否按了键盘上指定的键
	/*-------------自己加入的消息映射-----------*/
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMy18TetrisView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

void CMy18TetrisView::start() { //产生新图形
	rowPos = rowCount + 2;
	colPos = 1;
	kind = rand() % 7; //随机取形状
	for (int i = 0; i<4; ++i)
		for (int j = 0; j<2; ++j)
			shift[i][j] = RussiaData[kind][i][j]; //赋值，变成该种形状
}// 初始化方块变量

bool CMy18TetrisView::GoDown() { //按下方向键“下”的时候,或者计时器触发时
	for (int w = 0; w<4; ++w) {
		int i = (shift[w][0] + rowPos) / 2;
		int j = (shift[w][1] + colPos) / 2 + 1; //看看它的下一个格子是不是已经是地面了
		if (GameMap[i][j] == 1) return true; //落地返回true
	}
	colPos += 2;
	Invalidate();
	return false;
}
void CMy18TetrisView::trans() { //方块落地转化为地图
	for (int w = 0; w<4; ++w) {
		int i = (shift[w][0] + rowPos) / 2;
		int j = (shift[w][1] + colPos) / 2;
		GameMap[i][j] = 1;
	}
}
bool CMy18TetrisView::IsOver() { //判断到达顶端，游戏结束
	for (int i = 1; i <= rowCount; ++i)
		if (GameMap[i][0] == 1) return true;
	return false;
}

void CMy18TetrisView::GoLeft() { //按下方向键“左”的时候
	for (int w = 0; w<4; ++w) {
		int i = (shift[w][0] + rowPos) / 2 - 1;
		int j = (shift[w][1] + colPos) / 2;
		if (GameMap[i][j] == 1) return; //到左边界的话直接返回
	}
	rowPos -= 2;
	Invalidate();
}

void CMy18TetrisView::GoRight() {  //按下方向键“右”的时候
	for (int w = 0; w<4; ++w) {
		int i = (shift[w][0] + rowPos) / 2 + 1;
		int j = (shift[w][1] + colPos) / 2;
		if (GameMap[i][j] == 1) return; //到右边界的话直接返回
	}
	rowPos += 2;
	Invalidate();
}

void CMy18TetrisView::ShapeSwitch() { //按下方向键“上”的时候，改变形状
	int w;
	for (w = 0; w<4; ++w) {
		int i = (shift[w][1] + rowPos) / 2;
		int j = (-shift[w][0] + colPos) / 2;
		if (GameMap[i][j] == 1) return; //被卡住了，无法变形了
	}
	for (w = 0; w<4; ++w) { //逆时针旋转90度,只考虑
		shift[w][1] = shift[w][1] + shift[w][0];
		shift[w][0] = shift[w][1] - shift[w][0];
		shift[w][1] = -(shift[w][1] - shift[w][0]);
	}
	Invalidate();
}
void CMy18TetrisView::LineDelete() { //满了一行，把该行上方的全部方块集体往下移
	int cnt;
	int i = colCount;
	while (i) {
		cnt = 0;
		for (int j = 1; j <= rowCount; ++j)
			if (GameMap[j][i] == 1) ++cnt;
		if (cnt == rowCount) {
			for (int w = i; w > 0; w--)
				for (int j = 1; j <= rowCount; ++j)
					GameMap[j][w] = GameMap[j][w - 1];  //集体往下移动
		}
		else i--;
	}
	Invalidate();
}
// CMy18TetrisView 构造/析构

CMy18TetrisView::CMy18TetrisView()
{
	// TODO: 在此处添加构造代码
	rowCount = 12;//一行13个格子
	elen = 15;
	colCount = 20; //屏幕高20个格子
	block.LoadBitmap(IDB_BITMAP1);//CBitmap对象载入这个图片
	srand(time(0)); //这个干啥的呢？初始化随机种子？

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
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CMy18TetrisView 绘制

void CMy18TetrisView::OnDraw(CDC* pDC)
{
	CMy18TetrisDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
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
				CRect myrect(i*elen * 2, j*elen * 2, (i + 1)*elen * 2, (j + 1)*elen * 2); //矩形的四个定点
				pDC->FillRect(myrect, &background);
			}
			/*
			x：目标矩形区域的左上角x轴坐标点。
			y：目标矩形区域的左上角y轴坐标点。
			nWidth：在目标设备中绘制位图的宽度。
			nHight：在目标设备中绘制位图的高度。
			pSrcDC：源设备上下文对象指针。
			xSrc：源设备上下文的起点x轴坐标，函数从该起点复制位图到目标设备。
			ySrc：源设备上下文的起点y轴坐标，函数从该起点复制位图到目标设备。
			dwRop：光栅操作代码
			*/
			else  pDC->BitBlt(i*elen * 2, j*elen * 2, (i + 1)*elen * 2, (j + 1)*elen * 2, &Dc, 0, 0, SRCCOPY);
		}

	for (int w = 0; w<4; ++w) {
		int i = (shift[w][0] + rowPos) / 2;
		int j = (shift[w][1] + colPos) / 2;
		//该函数对指定的源设备环境区域中的像素进行位块（bit_block）转换，以传送到目标设备环境
		//src_copy 将源矩形区域直接拷贝到目标矩形区域
		if (i > 0 && j > 0) pDC->BitBlt(i*elen * 2, j*elen * 2, (i + 1)*elen * 2, (j + 1)*elen * 2, &Dc, 0, 0, SRCCOPY);
	}
}


// CMy18TetrisView 打印


void CMy18TetrisView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMy18TetrisView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CMy18TetrisView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CMy18TetrisView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
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


// CMy18TetrisView 诊断

#ifdef _DEBUG
void CMy18TetrisView::AssertValid() const
{
	CView::AssertValid();
}

void CMy18TetrisView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMy18TetrisDoc* CMy18TetrisView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMy18TetrisDoc)));
	return (CMy18TetrisDoc*)m_pDocument;
}
#endif //_DEBUG


// CMy18TetrisView 消息处理程序
void CMy18TetrisView::OnTimer(UINT nIDEvent) //计时器函数
{
	// TODO: Add your message handler code here and/or call default
	if (GoDown() == true) {
		trans(); //表示到达地面了
		if (IsOver()) {
			memset(GameMap, 1, sizeof(GameMap)); //游戏结束
		}
		else
		{
			LineDelete();//看看有没有行满足删除条件
			start(); //继续产生新图形
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
void CMy18TetrisView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) //按键按下的时候。触发该信息
{
	// TODO: Add your message handler code here and/or call default
	switch (nChar)
	{
	case VK_NUMPAD0://小数字键盘0
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