#pragma once

// محدد الخطأ
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

class ErrorArrow {
public:
    std::wstring error_arrow(std::wstring input_, Position positionStart, Position positionEnd) {
        std::wstring result;
        int columnStart, columnEnd;

        int newlineIndex = input_.rfind(L"\n", positionStart.index);
        int indexStart = std::max(newlineIndex, 0);
        int indexEnd = input_.find(L"\n", indexStart + 1);
        if (indexEnd < 0) {
            indexEnd = input_.length();
        }

        int lineCount = positionEnd.lineNumber - positionStart.lineNumber + 1;
        for (int i = 0; i < lineCount; i++) {
            std::wstring line = input_.substr(indexStart, indexEnd);
            if (i == 0) { columnStart = positionStart.columnNumber; }
            else { columnStart = 0; }
            if (i == lineCount - 1) { columnEnd = positionEnd.columnNumber + 1; }
            else { columnEnd = line.length() - 1; }

            result += line + L"\n" + std::wstring(columnStart, ' ') + std::wstring(columnEnd - columnStart, '^');

            indexStart = indexEnd;
            indexEnd = input_.find(L"\n", indexStart + 1);
            if (indexEnd < 0) { indexEnd = input_.length(); }
        }

        replace(result.begin(), result.end(), '\t', ' ');
        return result;
    }
};

// أخطاء
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Error {
public:
    Position positionStart, positionEnd;
    std::wstring errorName;
    wchar_t details;
    Error() {}
    Error(Position positionStart, Position positionEnd, std::wstring errorName, wchar_t details) : positionStart(positionStart), positionEnd(positionEnd), errorName(errorName), details(details) {}

    std::wstring print_() {
        std::wstring result = this->errorName + L": " + this->details + L"\n";
        result += L"الملف " + this->positionStart.fileName + L", السطر " + std::to_wstring(this->positionStart.lineNumber + 1);
        result += L"\n\n" + ErrorArrow().error_arrow(this->positionStart.input_, this->positionStart, this->positionEnd);

        return result;
    }
};

class SyntaxError : public Error {
public:
    SyntaxError(Position positionStart, Position positionEnd, wchar_t details) : Error(positionStart, positionEnd, L"خطأ في النسق", details) {
    }
};