#pragma once

class Fps {
public:
	Fps();
	~Fps();

	void Initialize();
	void Frame();
	int GetFps() const;

private:
	Fps(const Fps&);

	int m_fps;
	int m_count;
	uint64_t m_startTime;
};
