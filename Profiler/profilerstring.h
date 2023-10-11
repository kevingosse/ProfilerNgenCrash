#pragma once

#include <iostream>
#include <assert.h>
#include <cstring>
#include <string>
#include <algorithm>

#define WCHAR(str) L##str
#define CAST_CHAR(ch) ch

// 16 bit string type that works cross plat and doesn't require changing widths
// on non-windows platforms
class String
{
    friend std::wostream& operator<<(std::wostream& os, const String& obj);
private:
    WCHAR* buffer;
    size_t bufferLen;
    wchar_t* printBuffer;
    size_t printBufferLen;
    const size_t DefaultStringLength = 1024;

    void CopyBuffer(const WCHAR* other)
    {
        assert(other != nullptr);

        size_t otherLen = wcslen(other) + 1;
        if (buffer == nullptr || otherLen > bufferLen)
        {            
            bufferLen = std::max(DefaultStringLength, otherLen);
            if (buffer != nullptr)
            {
                delete[] buffer;
            }

            buffer = new WCHAR[bufferLen];
        }

        memcpy(buffer, other, otherLen * sizeof(WCHAR));
    }

public:
    String(const WCHAR* s = WCHAR("")) :
        buffer(nullptr),
        bufferLen(0),
        printBuffer(nullptr),
        printBufferLen(0)
    {
        CopyBuffer(s);
    }

    ~String()
    {
        if (buffer != nullptr)
        {
            bufferLen = 0;
            delete[] buffer;
        }

        if (printBuffer != nullptr)
        {
            printBufferLen = 0;
            delete[] printBuffer;
        }
    }

    String(const String& other) :
        buffer(nullptr),
        bufferLen(0),
        printBuffer(nullptr),
        printBufferLen(0)
    {
        CopyBuffer(other.buffer);
    }

    String(String&& other) noexcept :
        buffer(nullptr),
        bufferLen(0),
        printBuffer(nullptr),
        printBufferLen(0)
    {
        std::swap(buffer, other.buffer);
        std::swap(bufferLen, other.bufferLen);
    }

    String& operator=(const String& other)
    {
        if (this != &other)
        {
            if (other.buffer != nullptr)
            {
                CopyBuffer(other.buffer);
            }
        }

        printBuffer = nullptr;
        printBufferLen = 0;

        return *this;
    }

    String& operator=(String&& other) noexcept
    {
        std::swap(buffer, other.buffer);
        std::swap(bufferLen, other.bufferLen);

        printBuffer = nullptr;
        printBufferLen = 0;

        return *this;
    }

    bool operator==(const String& other) const
    {
        if (buffer == nullptr)
        {
            return buffer == other.buffer;
        }

        return wcscmp(buffer, other.buffer) == 0;
    }

    bool operator!=(const String& other) const
    {
        return !(*this == other);
    }

    String& operator+=(const String& other)
    {
        size_t currLen = wcslen(buffer);
        size_t otherLen = wcslen(other.buffer);
        size_t candidateLen = currLen + otherLen + 1;

        if (candidateLen > bufferLen)
        {
            WCHAR* newBuffer = new WCHAR[candidateLen];
            memcpy(newBuffer, buffer, currLen * sizeof(WCHAR));
            delete[] buffer;
            buffer = newBuffer;
        }

        memcpy(buffer + currLen, other.buffer, otherLen * sizeof(WCHAR));
        buffer[candidateLen - 1] = 0;
        return *this;
    }

    WCHAR& operator[] (size_t pos)
    {
        return buffer[pos];
    }

    const WCHAR& operator[] (size_t pos) const
    {
        return buffer[pos];
    }

    void Clear()
    {
        if (buffer != nullptr)
        {
            buffer[0] = 0;
        }
    }

    const wchar_t* ToCStr()
    {
        if (bufferLen == 0 || buffer == nullptr)
        {
            // Nothing to convert
            return nullptr;
        }

        if (bufferLen > printBufferLen)
        {
            if (printBuffer != nullptr)
            {
                delete[] printBuffer;
            }

            printBuffer = new wchar_t[bufferLen];
            printBufferLen = bufferLen;
        }

        for (size_t i = 0; i < bufferLen; ++i)
        {
            printBuffer[i] = CAST_CHAR(buffer[i]);
        }

        // Make sure it's null terminated
        printBuffer[bufferLen - 1] = '\0';

        return printBuffer;
    }

    std::wstring ToWString()
    {
        std::wstring temp;
        for (size_t i = 0; i < bufferLen; ++i)
        {
            if (buffer[i] == 0)
            {
                break;
            }

            temp.push_back(CAST_CHAR(buffer[i]));
        }

        return temp;
    }

    size_t Length() const
    {
        return wcslen(buffer);
    }
};

inline std::wostream& operator<<(std::wostream& os, const String& obj)
{
    for (size_t i = 0; i < obj.bufferLen; ++i)
    {
        if (obj.buffer[i] == 0)
        {
            break;
        }

        os << CAST_CHAR(obj.buffer[i]);
    }

    return os;
}

inline bool EndsWith(const char* lhs, const char* rhs)
{
    size_t lhsLen = strlen(lhs);
    size_t rhsLen = strlen(rhs);
    if (lhsLen < rhsLen)
    {
        return false;
    }

    size_t lhsPos = lhsLen - rhsLen;
    size_t rhsPos = 0;

    while (rhsPos < rhsLen)
    {
        if (lhs[lhsPos] != rhs[rhsPos])
        {
            return false;
        }

        ++lhsPos;
        ++rhsPos;
    }

    return true;
}

inline bool EndsWith(const String& lhs, const String& rhs)
{
    size_t lhsLength = lhs.Length();
    size_t rhsLength = rhs.Length();
    if (lhsLength < rhsLength)
    {
        return false;
    }

    size_t lhsPos = lhsLength - rhsLength;
    size_t rhsPos = 0;

    while (rhsPos < rhsLength)
    {
        if (std::tolower(lhs[lhsPos]) != std::tolower(rhs[rhsPos]))
        {
            return false;
        }

        ++lhsPos;
        ++rhsPos;
    }

    return true;
}