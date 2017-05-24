#include "pch.h"
#include "Fps.h"

Fps::Fps() : m_fps(0),
m_count(0),
m_startTime(0) {}

Fps::Fps(const Fps&) :
	m_fps(0), m_count(0),
	m_startTime(0) {}

Fps::~Fps() {}

void Fps::Initialize() {
	m_fps = 0;
	m_count = 0;
	m_startTime = timeGetTime();
}

void Fps::Frame() {
	m_count++;

	if (timeGetTime() >= m_startTime + 1000) {
		m_fps = m_count;
		m_count = 0;

		m_startTime = timeGetTime();
	}
}

int Fps::GetFps() const {
	return m_fps;
}
