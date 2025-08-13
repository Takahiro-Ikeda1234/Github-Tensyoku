//=============================================================================
//
// [Counter.h] 時間カウント用クラス
//
//=============================================================================
#ifndef _COUNTER_H_
#define _COUNTER_H_

//クラス定義
class Counter
{
public:
	Counter() : mMax(0), mLeft(0), mIsFinish(false) {}
	Counter(float time) : mMax(time), mLeft(time), mIsFinish(false) {}
	virtual ~Counter() {}
	virtual bool IsEnd();
	virtual bool IsFinish();
	float Percentage() { if (mMax == 0)return 0; return mLeft / mMax; }
	float ElapsedTime() { return mMax - mLeft; }
	virtual Counter& operator =(float time) { mLeft = mMax = time; mIsFinish = false; return *this; }
	virtual Counter& operator -=(float deltaTime);
	virtual Counter& operator +=(float deltaTime);
	float GetLeftTime() { return mLeft; }
	float GetMaxTime() { return mMax; }
	void SetStatus(float max, float left) { mLeft = left; mMax = max; if (mMax == 0 || mLeft == 0) mIsFinish = true; }
	virtual Counter& operator =(Counter& ct) { SetStatus(ct.mMax, ct.mLeft); mIsFinish = ct.mIsFinish; return *this; }
	void Reverse() { mLeft = mMax - mLeft; };
	virtual Counter& operator*=(float scale) { mMax *= scale; mLeft *= scale; return *this; }
protected:
	float mMax;	//時間
	float mLeft;	//残り時間
	bool mIsFinish;	//終了した瞬間かの判定用
};
class LoopCounter : public Counter
{
public:
	LoopCounter() : Counter() {}
	LoopCounter(float time) : Counter(time) {}
	~LoopCounter() {}
	LoopCounter& operator = (float time) { mLeft = mMax = time; mIsFinish = false; return *this; }
	LoopCounter& operator -=(float deltaTime) override;
	bool IsFinish() override { return mIsFinish; }
	bool IsEnd()override { return mIsFinish; }
private:
};

#endif