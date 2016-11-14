#pragma once

class GameTimer
{
public:
	GameTimer();
	~GameTimer();

	bool Initialize();
	void Frame();

	float GetTime() const;

	void StartTimer();
	void StopTimer();
	int GetTiming() const;

private:
	GameTimer(const GameTimer&);

	float m_frequency;
	INT64 m_startTime;
	float m_frameTime;
	INT64 m_beginTime;
	INT64 m_endTime;
};
