/**
 *  Copyright (C) 2019
 *  Author Alvin Ahmadov <alvin.dev.ahmadov@gmail.com>
 */
#pragma once

#define PROJECT_VERSION  "1.0.0"

#define ORGANIZATION    "None"
#define BACKEND_URL     "https://backend.facecloud.tevian.ru"
#define API_PATH        "/api/v1/openapi.json"
#define PROJECT_NAME    "TevianTask"

#define TEVIAN_EXPORT

#ifdef TEVIAN_EXPORT
#  define TEVIAN_API Q_DECL_EXPORT
#elif defined(TEVIAN_IMPORT)
#  define TEVIAN_API Q_DECL_IMPORT
#endif

#ifdef PRINT_DEBUG
#ifndef EXCEPTION_DETAIL
#define EXCEPTION_DETAIL __FILE__ << ":" << __LINE__
#endif

#define prints0(str)                                        std::cout<<str
#define prints_fmt0(str, ios_fmt)                           std::cout<<(ios_fmt)<<(str)
#define prints(str)                                         prints0(str);
#define prints_fmt(str, fmt)                                prints_fmt0(str);
#define print_log0(str)                                     std::cout<< "Log: " << __FUNCTION__ << "() " << (str)
#define print_log_fmt0(str, ios_fmt)                        std::cout<< "Log: " << __FUNCTION__ << "() " << (ios_fmt) << (str)
#define print_log(str)                                      print_log0(str);
#define print_log_fmt(str, ios_fmt)                         print_log_fmt0(str, ios_fmt);
#define printl_log(str)                                     print_log0(str) << std::endl;
#define printl_log_fmt(str, ios_fmt)                        print_log_fmt0(str, ios_fmt) << std::endl;
#define printw(str)                                         std::wcout<<str;
#define printw_fmt(str, fmt)                                std::wcout<< fmt << str;
#define printc(str)                                         std::cout<<str.c_str();
#define printl(str)                                         prints0(str) <<std::endl;
#define printl_fmt(str, ios_fmt)                            prints_fmt0(str, ios_fmt) <<std::endl;
#define prints_debug(str, ...)                              auto __strng = std::string(str); __strng.insert(0, "%s:%d Debugging: "); \
                                                            std::printf(__strng.c_str(), __FILE__, __LINE__, args);
#define printl_debug(str, ...)                              prints_debug(str, args) printl("")
#define printf_debug(str, ...)                              std::cout<<str<<...;
#define printlw(str)                                        std::wcout<<str<<std::endl;
#define printlc(str)                                        printl(str.c_str())
#define printf_(str)                                        if (!str.empty()) {PUT(str)}
#define printlf(str)                                        if (!str.empty()) {PUTL(str)}
#define printn(Object)                                      print(#Object)
#define printlv(value)                                      printl(xstr(value:) << " " << value)
#define printlvu(value)                                     print(xstr(value:)) P_ printlc(value)
#define printv(value)                                       print(xstr(value:)) print(" ") print(value) print(" ")
#define printb(booleanExpr)                                 printl((booleanExpr ? "True" : "False"))
#define printvar(var) print("\t")                           print(xstr(var))
#define printvarl(var) print("\t")                          printl(xstr(var))
#define printbe(str, booleanExpr, expr_true, expr_false)    std::cout<< str << " " << (booleanExpr ? expr_true : expr_false) << std::endl;
#define printbs(str, booleanExpr)                           printbe(str, booleanExpr, "True", "False");
#define printerr_detail(error_msg)                          std::cerr<<__FILE__ << ":" << __LINE__<<' '<<error_msg<<std::endl;
#define printerr(error_msg)                                 printerr_detail(error_msg)

#ifdef EXCEPTION_DETAIL
#undef EXCEPTION_DETAIL
#endif
#else
#define prints0(str)
#define prints_fmt0(str, ios_fmt)
#define prints(str)
#define prints_fmt(str, fmt)
#define print_log0(str)
#define print_log_fmt0(str, ios_fmt)
#define print_log(str)
#define print_log_fmt(str, ios_fmt)
#define printl_log(str)
#define printl_log_fmt(str, ios_fmt)
#define printw(str)
#define printw_fmt(str, fmt)
#define printc(str)
#define printl(str)
#define printl_fmt(str, ios_fmt)
#define printlw(str)
#define printlc(str)
#define printf_(str)
#define printlf(str)
#define printn(Object)
#define printlv(value)
#define printlvu(value)
#define printv(value)
#define printb(booleanExpr)
#define printvar(var) print("\t")
#define printvarl(var) print("\t")
#define printbe(str, booleanExpr, expr_true, expr_false)
#define printbs(str, booleanExpr)
#define printerr_detail(error_msg)
#define printerr(error_msg)
#endif //PRINT_DEBUG

#ifndef DECL_BASE_DETAILED_EXCEPTION
#define DECL_BASE_DETAILED_EXCEPTION(exceptionBase, className)       \
    class className : public exceptionBase                           \
    {                                                                \
    public:                                                          \
        className();                                                 \
        explicit className(const char* message);                     \
        explicit className(const QString& message);                  \
        template <typename ... Args>                                 \
        className(const char* message, Args&& ... args)              \
        {                                                            \
           m_message = QString::asprintf(message,                    \
           		std::forward<Args>(args)...);                        \
        }                                                            \
        ~className() Q_DECL_NOEXCEPT Q_DECL_OVERRIDE;                \
        const QString getMessage() const Q_DECL_NOEXCEPT             \
                                         Q_DECL_OVERRIDE;            \
    };

#endif  //DECL_BASE_DETAILED_EXCEPTION

#ifndef DEF_BASE_DETAILED_EXCEPTION
#define DEF_BASE_DETAILED_EXCEPTION(exceptionBase, className)       \
    className::className()                                          \
        : exceptionBase()                                           \
    {}                                                              \
    className::className(const char* msg)                           \
        : exceptionBase(msg)                                        \
    {}                                                              \
    className::className(const QString& msg)                        \
        : exceptionBase(msg)                                        \
    {}                                                              \
    className::~className() Q_DECL_NOEXCEPT                         \
    {}                                                              \
    const QString className::getMessage() const Q_DECL_NOEXCEPT     \
    {                                                               \
        return exceptionBase::getMessage();                         \
    }                                                               \

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)  if(p) delete p;
#endif

#endif  //DEF_BASE_DETAILED_EXCEPTION