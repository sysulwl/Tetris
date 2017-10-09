
// 18.TetrisView.h : CMy18TetrisView 类的接口
//

#pragma once


class CMy18TetrisView : public CView
{
protected: // 仅从序列化创建
	CMy18TetrisView();
	DECLARE_DYNCREATE(CMy18TetrisView)

// 特性
public:
	CMy18TetrisDoc* GetDocument() const;

// 操作
public:
/*---------------------自己加入的类成员-----------------*/
public:
	void start();   // 初始化方块变量，生成图形
	void trans(); //  方块落地转化为地图
	void LineDelete();   // 除去都是方块的行 
	void ShapeSwitch(); // 变形 旋转
	bool GoDown();     // 下落 落地返回true
	void GoLeft();    // 左移
	void GoRight();  // 右移
	bool IsOver();  // 是否游戏失败了
private:
	int rowCount;   // 横格个数
	int colCount;  // 竖格个数
	int elen;     // 半格长度
	bool GameMap[30][30];
	int rowPos, colPos;         // 方块中心坐标
	int shift[4][2];          // 相对中心的偏移量  半格为单位
	int kind;                // 类型
	CBitmap block;         //方块位图
 /*---------------------自己加入的类成员-----------------*/
// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CMy18TetrisView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
/*-----------------自己加入的消息映射函数-----------------*/
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags); //啊，就是这个函数，控制了这个消息传递啊！！
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
/*-----------------自己加入的消息映射函数-----------------*/
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // 18.TetrisView.cpp 中的调试版本
inline CMy18TetrisDoc* CMy18TetrisView::GetDocument() const
   { return reinterpret_cast<CMy18TetrisDoc*>(m_pDocument); }
#endif

