#include "pch.h"
#include "GameTimer.h"

GameTimer::GameTimer() :
	m_frequency(0),
	m_startTime(0),
	m_frameTime(0),
	m_beginTime(0),
	m_endTime(0) {}

GameTimer::GameTimer(const GameTimer&) :
	m_frequency(0),
	m_startTime(0),
	m_frameTime(0),
	m_beginTime(0),
	m_endTime(0) {}

GameTimer::~GameTimer() {}

bool GameTimer::Initialize() {
	INT64 frequency;
	// Get the cycles per second speed for this system.
	QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&frequency));
	if (frequency == 0) {
		return false;
	}

	// Store it in floating point.
	m_frequency = static_cast<float>(frequency);

	// Get the initial start time.
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&m_startTime));

	return true;
}

void GameTimer::Frame() {
	// Query the current time.
	INT64 currentTime;
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currentTime));

	// Calculate the difference in time since the last time we queried for the current time.
	INT64 elapsedTicks = currentTime - m_startTime;

	// Calculate the frame time.
	m_frameTime = static_cast<float>(elapsedTicks) / m_frequency;

	// Restart the timer.
	m_startTime = currentTime;
}

float GameTimer::GetTime() const {
	return m_frameTime;
}

void GameTimer::StartTimer() {
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&m_beginTime));
}

void GameTimer::StopTimer() {
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&m_endTime));
}

int GameTimer::GetTiming() const {
	// Get the elapsed ticks between the two times.
	float elapsedTicks = static_cast<float>(m_endTime - m_beginTime);

	// Get the ticks per second speed of the timer
	INT64 frequency;
	QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&frequency));

	// Calculate the elapsed time in milliseconds.
	float milliseconds = elapsedTicks / static_cast<float>(frequency) * 1000.0f;

	return static_cast<int>(milliseconds);
}
