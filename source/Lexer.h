#pragma once

// تم تعريف المتغيرات خارج الصنف لكي لا يتم إعادة ضبطها عند استخدام الطرفية في تنفيذ الشفرة
int name = 0; // متغير اسماء ولكن على شكل ارقام
std::map<STR, int> namesAlter = {};

// المعرب اللغوي
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct DedentSpecifier { // صنف يقوم بتحديد المسافة البادئة الحالية والاخيرة
    int spaces = 0;
    DedentSpecifier* previous = nullptr;
};

class Lexer {
public:
    STR fileName{}, input_{};
    wchar_t currentChar{};
    Position position_{}, positionEnd{};
    std::vector<Token> tokens_{};

    // vars
    ////////////

    DedentSpecifier* dedentSpec = new DedentSpecifier; // حساب المسافات البادئة والنهائية

    ////////////

    Lexer(STR _fileName, STR _input)
    {
        this->fileName = _fileName;
        this->input_ = _input;
        this->position_ = Position(-1, 0, -1);
        this->currentChar = L'\0';
        this->advance();
    }

    void advance() {
        this->position_.advance(this->currentChar);

        if (this->position_.index_ < this->input_.length())
        {
            this->currentChar = this->input_[this->position_.index_];
        }
        else {
            this->currentChar = L'\0';
        }
    }

    void make_token() {

        while (this->currentChar != L'\0')
        {
            if (this->currentChar == L'\r')
            {
                this->advance();
            }
            else if (this->currentChar == L'\n') // يجب ان يتم التحقق من السطر الجديد قبل المسافة او المسافة البادئة
            {
                this->make_newline();
            }
            else if (this->currentChar == L' ' or this->currentChar == L'\t')
            {
                this->skip_space();
            }
            else if (this->currentChar == L'#')
            {
                this->skip_comment();
            }
            else if (digits.find(this->currentChar) != STR::npos)
            {
                this->make_number();
            }
            else if (this->currentChar == L'.') {
                Position positionStart = this->position_;
                this->advance();
                this->tokens_.push_back(Token(positionStart, this->position_, TTdot));
            }
            else if (letters.find(this->currentChar) != std::wstring::npos) {
                this->make_name();
            }
            else if (this->currentChar == L'\"')
            {
                this->make_string();
            }
            else if (this->currentChar == L'+')
            {
                this->make_plus_equal();
            }
            else if (this->currentChar == L'-')
            {
                this->make_minus_equal();
            }
            else if (this->currentChar == L'*')
            {
                this->make_multiply_equal();
            }
            else if (this->currentChar == L'\\')
            {
                this->make_divide();
            }
            else if (this->currentChar == L'^')
            {
                this->make_power_equal();
            }
            else if (this->currentChar == L'(')
            {
                Position positionStart = this->position_;
                this->advance();
                this->tokens_.push_back(Token(positionStart, this->position_, TTlParenthesis));
            }
            else if (this->currentChar == L')')
            {
                Position positionStart = this->position_;
                this->advance();
                this->tokens_.push_back(Token(positionStart, this->position_, TTrParenthesis));
            }
            else if (this->currentChar == L'[')
            {
                Position positionStart = this->position_;
                this->advance();
                this->tokens_.push_back(Token(positionStart, this->position_, TTlSquare));
            }
            else if (this->currentChar == L']')
            {
                Position positionStart = this->position_;
                this->advance();
                this->tokens_.push_back(Token(positionStart, this->position_, TTrSquare));
            }
            else if (this->currentChar == L':')
            {
                Position positionStart = this->position_;
                this->advance();
                this->tokens_.push_back(Token(positionStart, this->position_, TTcolon));
            }
            else if (this->currentChar == L',')
            {
                Position positionStart = this->position_;
                this->advance();
                tokens_.push_back(Token(positionStart, this->position_, TTcomma));
            }
            else if (this->currentChar == L'!')
            {
                this->make_not_equals();
            }
            else if (this->currentChar == L'=')
            {
                this->make_equals();
            }
            else if (this->currentChar == L'<')
            {
                this->make_less_than();
            }
            else if (this->currentChar == L'>')
            {
                this->make_greater_than();
            }
            else
            {
                STR detail = L"< حرف غير معروف \'";
                detail.push_back(this->currentChar);
                detail += L"\' >";

                prnt(SyntaxError(this->position_, this->position_, detail, fileName, input_).print_());
                exit(0);
            }
        }

        tokens_.push_back(Token(this->position_, this->position_, TTendOfFile));

    }


    void skip_space()
    {
        if (this->position_.line_ == 0 and this->position_.column_ == 0)
        {
            this->make_indent();
        }
        else
        {
            while (this->currentChar == L' ' or this->currentChar == L'\t')
            {
                this->advance();
            }
        }
    }

    void make_indent()
    {
        Position positionStart = this->position_;
        int spaces = 0;

        while (this->currentChar == L'\t' or this->currentChar == L' ')
        {
            if (this->currentChar == L'\t')
            {
                this->advance();
                spaces += 4;
            }
            else
            {
                this->advance();
                spaces++;
            }
        }

        if (spaces > dedentSpec->spaces)
        {
            if (this->currentChar != L'\n') // تحقق اذا كان السطر لا يحتوي سوى مسافات بادئة >> قم بتخطيه
            {
                this->tokens_.push_back(Token(positionStart, this->position_, TTindent, spaces));
                DedentSpecifier* newIndent = new DedentSpecifier(*dedentSpec);
                dedentSpec->spaces = spaces;
                dedentSpec->previous = newIndent;
            }
            else
            {
                this->advance();
            }
        }
        else if (spaces < dedentSpec->spaces)
        {
            while (this->dedentSpec->spaces != spaces) {

                if (this->dedentSpec->spaces < spaces)
                {
                    prnt(L"خطأ في المسافات البادئة - لقد خرجت عن النطاق الحالي");
                    exit(-1);
                }

                if (this->dedentSpec->previous != nullptr)
                {
                    this->dedentSpec = this->dedentSpec->previous;

                }
                else {
                    prnt(L"خطأ في المسافات البادئة - لقد خرجت عن النطاق الحالي");
                    exit(-1);
                }


                this->tokens_.push_back(Token(positionStart, this->position_, TTdedent, spaces));
            }
        }
    }

    void make_newline()
    {
        Position positionStart = this->position_;
        this->advance();
        this->tokens_.push_back(Token(positionStart, this->position_, TTnewline));

        this->make_indent();
    }

    void make_number() {
        STR numberString = L"";
        unsigned int dotCount = 0;
        Position positionStart = this->position_;

        while (this->currentChar != L'\0' and (digits + L".").find(this->currentChar) != STR::npos) {
            if (this->currentChar == L'.') {
                if (dotCount == 1) {
                    dotCount++;
                    break;
                }
                dotCount++;
            }
            numberString += this->currentChar;
            this->advance();
        }

        if (dotCount == 0)
        {
            this->tokens_.push_back(Token(positionStart, this->position_, TTinteger, std::stoi(numberString)));

        }
        else if (dotCount == 1) {
            this->tokens_.push_back(Token(positionStart, this->position_, TTfloat, std::stod(numberString)));
        }
        else
        {
            STR detail = L"< ";
            detail.push_back(this->currentChar);
            detail += L" >";

            prnt(SyntaxError(this->position_, this->position_, detail, fileName, input_).print_());
            exit(0);
        }
    }

    void make_name()
    {
        STR nameString;
        Position positionStart = this->position_;

        while (this->currentChar != L'\0' and (lettersDigits + L'_').find(this->currentChar) != STR::npos) {
            nameString += this->currentChar;
            this->advance();
        }

        if (keywords_.find(nameString) != keywords_.end())
        {
            this->tokens_.push_back(Token(positionStart, this->position_, TTkeyword, keywords_[nameString]));
        }
        else if (buildInFunctions.find(nameString) != buildInFunctions.end())
        {
            this->tokens_.push_back(Token(positionStart, this->position_, TTbuildInFunc, buildInFunctions[nameString]));
        }
        else if (namesAlter.find(nameString) != namesAlter.end())
        {
            this->tokens_.push_back(Token(positionStart, this->position_, TTname, namesAlter[nameString]));
        }
        else
        {
            name++;
            namesAlter[nameString] = name;
            this->tokens_.push_back(Token(positionStart, this->position_, TTname, name));
        }
    }

    void make_string()
    {
        STR string_ = L"";
        Position positionStart = this->position_;
        bool ClosedString = true;
        this->advance();

        while (this->currentChar != L'\"') {
            if (this->currentChar == L'\0' or this->currentChar == L'\n') {
                ClosedString = false;
                break;
            }
            else {
                string_ += this->currentChar;
                this->advance();
            }
        }

        if (ClosedString)
        {
            this->advance();
            STR* newString = new STR(string_);
            this->tokens_.push_back(Token(positionStart, this->position_, TTstring, newString));
        }
        else {
            prnt(SyntaxError(positionStart, this->position_, L"< لم يتم إغلاق النص >", fileName, input_).print_());
            exit(0);
        }
    }

    void make_plus_equal(){
        Position positionStart = this->position_;
        this->advance();

        if (this->currentChar == L'=') {
            this->advance();
            this->tokens_.push_back(Token(positionStart, this->position_, TTplusEqual));
        }
        else {
            this->tokens_.push_back(Token(positionStart, this->position_, TTplus));
        }
    }

    void make_minus_equal() {
        Position positionStart = this->position_;
        this->advance();

        if (this->currentChar == L'=') {
            this->advance();
            this->tokens_.push_back(Token(positionStart, this->position_, TTminusEqual));
        }
        else {
            this->tokens_.push_back(Token(positionStart, this->position_, TTminus));
        }
    }

    void make_multiply_equal() {
        Position positionStart = this->position_;
        this->advance();

        if (this->currentChar == L'=') {
            this->advance();
            this->tokens_.push_back(Token(positionStart, this->position_, TTmultiplyEqual));
        }
        else {
            this->tokens_.push_back(Token(positionStart, this->position_, TTmultiply));
        }
    }

    void make_power_equal() {
        Position positionStart = this->position_;
        this->advance();

        if (this->currentChar == L'=') {
            this->advance();
            this->tokens_.push_back(Token(positionStart, this->position_, TTpowerEqual));
        }
        else {
            this->tokens_.push_back(Token(positionStart, this->position_, TTpower));
        }
    }

    void make_divide() {
        Position positionStart = this->position_;
        this->advance();

        if (this->currentChar == L'=') {
            this->advance();
            this->tokens_.push_back(Token(positionStart, this->position_, TTdivideEqual));
        }
        else if (this->currentChar == L'\\') {
            this->advance();
            this->tokens_.push_back(Token(positionStart, this->position_, TTremain));
        }
        else {
            this->tokens_.push_back(Token(positionStart, this->position_, TTdivide));
        }
    }

    void make_not_equals() {
        Position positionStart = this->position_;
        this->advance();

        if (this->currentChar == L'=') {
            this->advance();
            this->tokens_.push_back(Token(positionStart, this->position_, TTnotEqual));
        }
        else {
            prnt(SyntaxError(this->position_, this->position_, L"< يتوقع وجود \'=\' بعد إشارة \'!\' >", fileName, input_).print_());
            exit(0);
        }
    }

    void make_equals() {
        Position positionStart = this->position_;
        this->advance();

        if (this->currentChar == L'=') {
            this->advance();
            this->tokens_.push_back(Token(positionStart, this->position_, TTequalEqual));
        }
        else
        {
            this->tokens_.push_back(Token(positionStart, this->position_, TTequal));
        }
    }

    void make_less_than() {
        Position positionStart = this->position_;
        this->advance();

        if (this->currentChar == L'=') {
            this->advance();
            this->tokens_.push_back(Token(positionStart, this->position_, TTlessThanEqual));
        }
        else {
            this->tokens_.push_back(Token(positionStart, this->position_, TTlessThan));
        }

    }

    void make_greater_than() {
        Position positionStart = this->position_;
        this->advance();

        if (this->currentChar == L'=') {
            this->advance();
            this->tokens_.push_back(Token(positionStart, this->position_, TTgreaterThanEqual));
        }
        else
        {
            this->tokens_.push_back(Token(positionStart, this->position_, TTgreaterThan));
        }
    }

    void skip_comment() {
        this->advance();
        while (this->currentChar != L'\n' and this->currentChar != L'\0') {
            this->advance();
        }
    }
};
