#pragma once

class Fps
{
public:
	Fps();
	~Fps();

	void Initialize();
	void Frame();
	int GetFps() const;

private:
	Fps(const Fps&);

	int m_fps, m_count;
	unsigned long m_startTime;	
};
