#pragma once

#include <exception>

namespace DX
{
	inline void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
	{
		char* compileErrors = static_cast<char*>(errorMessage->GetBufferPointer());
		unsigned long long bufferSize = errorMessage->GetBufferSize();
		std::ofstream fout;
		fout.open("shader-error.txt");
		for (unsigned long long i = 0; i < bufferSize; i++)
		{
			fout << compileErrors[i];
		}

		fout.close();
		MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);
	}

	class ComException : public std::exception
	{
	public:
		explicit ComException(HRESULT hr) : result(hr) {}

		const char* what() const override
		{
			static char s_str[64] = {};
			sprintf_s(s_str, "Failure with HRESULT of %08X", result);
			return s_str;
		}

	private:
		HRESULT result;
	};

	inline void ThrowIfFailed(HRESULT hr)
	{
		if (FAILED(hr))
		{
			throw ComException(hr);
		}
	}

	inline void DebugTrace(_In_z_ _Printf_format_string_ const char* format, ...)
	{
#ifdef _DEBUG
		va_list args;
		va_start(args, format);

		char buff[1024] = {};
		vsprintf_s(buff, format, args);
		OutputDebugStringA(buff);
		va_end(args);
#else
		UNREFERENCED_PARAMETER(format);
#endif
	}	
}
