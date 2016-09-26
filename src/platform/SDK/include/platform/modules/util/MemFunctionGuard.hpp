/**************************************************************************************************
Copyright(C) 2006, Huawei Tech. Co., Ltd.                
ALL RIGHTS RESERVED                                      

文件名称:  MemFunctionGuard.hpp                         
作    者:  Jerry.An  43252                                          
文件版本:  1.0.0                                             
创建日期:  2007/07/23
描述信息:  MemFunctionGuard类的定义及实现.
修改记录:   
**************************************************************************************************/
#ifndef _CME_UTILS_EXT_FUNC_GUARD_MAP_HEADER_
#define _CME_UTILS_EXT_FUNC_GUARD_MAP_HEADER_

#include "Noncopyable.hpp"

namespace UtilsExt{

class OperationGuard : private Noncopyable
{
public:
    void releaseControl(void) {
        control_ = false;
    }

protected:
    OperationGuard() : control_(true) {}

    bool control_;
};

template<typename A, typename R = void>
class MemFunGuardWith1Arg : public OperationGuard
{
    typedef R (*MemFun)(A);
public:
    MemFunGuardWith1Arg(MemFun _fun, A _arg)
        : arg_(_arg),fun_(_fun)
    {
    }

    /*lint --e(1509)*/
    ~MemFunGuardWith1Arg() throw() 
    {
        try 
        {
            if (control_) 
            {
                (*fun_)(arg_);
            }
        }
        catch(...) 
        {
        }
    }
private:
    A arg_;
    MemFun fun_;
};

template<typename A, typename R = void>
class MemFunGuardWithRefArg : public OperationGuard
{
    typedef R (*MemFun)(A&);
public:
    MemFunGuardWithRefArg(MemFun _fun, A& _arg)
        : arg_(_arg),fun_(_fun)
    {
    }

    /*lint --e(1509)*/
    ~MemFunGuardWithRefArg() throw() 
    {
        try 
        {
            if (control_) 
            {
                (*fun_)(arg_);
            }
        }
        catch(...) 
        {
        }
    }
private:
    A& arg_;
    MemFun fun_;
};

template<typename A, typename R = void>
class MemFunGuardWithConstRefArg : public OperationGuard
{
    typedef R (*MemFun)(const A&);
public:
    MemFunGuardWithConstRefArg(MemFun _fun, const A& _arg)
        :arg_(_arg),fun_(_fun)
    {
    }

    /*lint --e(1509)*/
    ~MemFunGuardWithConstRefArg() throw() 
    {
        try 
        {
            if (control_) 
            {
                (*fun_)(arg_);
            }
        }
        catch(...) 
        {
        }
    }
private:
    const A& arg_;
    MemFun fun_;
};

template<class T, typename ArgT1, typename ArgT2, typename R = void>
class ClassMemFunGuardWith2Arg : public OperationGuard
{
    typedef R (T::*MemFun)(ArgT1, ArgT2);
public:
    ClassMemFunGuardWith2Arg(T& obj, ArgT1 _arg1, ArgT2 _arg2, MemFun fun)
        : arg1_(_arg1), arg2_(_arg2), obj_(obj), fun_(fun)
    {
    }

    /*lint --e(1509)*/
    ~ClassMemFunGuardWith2Arg() throw() {
        try {
            if (control_) {
                (obj_.*fun_)(arg1_, arg2_);
            }
        }
        catch(...) {
        }
    }

private:
    ArgT1 arg1_;
    ArgT2 arg2_;
    T& obj_;
    MemFun fun_;
};

template<class T, typename ArgT1, typename ArgT2, typename R = void>
class ClassConstMemFunGuardWith2Arg : public OperationGuard
{
    typedef R (T::*ConstMemFun)(ArgT1, ArgT2) const;
public:
    ClassConstMemFunGuardWith2Arg(T const & obj, ArgT1 _arg1, ArgT2 _arg2, ConstMemFun fun)
        : arg1_(_arg1), arg2_(_arg2), obj_(obj), fun_(fun) {
        }

        /*lint --e(1509)*/
        ~ClassConstMemFunGuardWith2Arg() throw() {
            try {
                if (control_) {
                    (obj_.*fun_)(arg1_, arg2_);
                }
            }
            catch(...) {
            }
        }

private:
    ArgT1 arg1_;
    ArgT2 arg2_;
    T const & obj_;
    ConstMemFun fun_;
};

template<class T, typename A, typename R = void>
class ClassMemFunGuardWithRefArg : public OperationGuard
{
    typedef R (T::*MemFun)(A&);
public:
    ClassMemFunGuardWithRefArg(T& obj, MemFun _fun, A& _arg)
        : obj_(obj), arg_(_arg),fun_(_fun)
    {
    }

    /*lint --e(1509)*/
    ~ClassMemFunGuardWithRefArg() throw() 
    {
        try 
        {
            if (control_) 
            {
                (obj_.*fun_)(arg_);
            }
        }
        catch(...) 
        {
        }
    }
private:
    T& obj_;
    A& arg_;
    MemFun fun_;
};

template<class T, typename A, typename R = void>
class ClassMemFunGuardWithArg1 : public OperationGuard
{
    typedef R (T::*MemFun)(A);
public:
    ClassMemFunGuardWithArg1(T& obj, MemFun _fun, A _arg)
        : obj_(obj), arg_(_arg),fun_(_fun)
    {
    }

    /*lint --e(1509)*/
    ~ClassMemFunGuardWithArg1() throw() 
    {
        try 
        {
            if (control_) 
            {
                (obj_.*fun_)(arg_);
            }
        }
        catch(...) 
        {
        }
    }
private:
    T& obj_;
    A arg_;
    MemFun fun_;
};

template<class T, typename A, typename R = void>
class ClassMemFunGuardWithConstRefArg : public OperationGuard
{
    typedef R (T::*MemFun)(const A&);
public:
    ClassMemFunGuardWithConstRefArg(T& obj, MemFun _fun, const A& _arg)
        : obj_(obj), arg_(_arg),fun_(_fun)
    {
    }

    /*lint --e(1509)*/
    ~ClassMemFunGuardWithConstRefArg() throw() 
    {
        try 
        {
            if (control_) 
            {
                (obj_.*fun_)(arg_);
            }
        }
        catch(...) 
        {
        }
    }
private:
    T& obj_;
    const A& arg_;
    MemFun fun_;
};

template<class T, typename A, typename R = void>
class ClassConstMemFunGuardWithConstRefArg : public OperationGuard
{
    typedef R (T::*MemFun)(const A&) const;
public:
    ClassConstMemFunGuardWithConstRefArg(T& obj, MemFun _fun, const A& _arg)
        : obj_(obj), arg_(_arg),fun_(_fun)
    {
    }

    /*lint --e(1509)*/
    ~ClassConstMemFunGuardWithConstRefArg() throw() 
    {
        try 
        {
            if (control_) 
            {
                (obj_.*fun_)(arg_);
            }
        }
        catch(...) 
        {
        }
    }
private:
    T& obj_;
    const A& arg_;
    MemFun fun_;
};

template<class T, typename A, typename R = void>
class ConstClassConstMemFunGuardWithConstRefArg : public OperationGuard
{
    typedef R (T::*MemFun)(const A&) const;
public:
    ConstClassConstMemFunGuardWithConstRefArg(const T& obj, MemFun _fun, const A& _arg)
        : obj_(obj), arg_(_arg),fun_(_fun)
    {
    }

    /*lint --e(1509)*/
    ~ConstClassConstMemFunGuardWithConstRefArg() throw() 
    {
        try 
        {
            if (control_) 
            {
                (obj_.*fun_)(arg_);
            }
        }
        catch(...) 
        {
        }
    }
private:
    const T& obj_;
    const A& arg_;
    MemFun fun_;
};

template<class T, typename A, typename B, typename R = void>
class ClassMemFunGuardWith2RefArg : public OperationGuard
{
    typedef R (T::*MemFun)(A&, B&);
public:
    ClassMemFunGuardWith2RefArg(T& obj, MemFun _fun, A& _arg1, B& _arg2)
        : obj_(obj), arg1_(_arg1), arg2_(_arg2), fun_(_fun)
    {
    }

    /*lint --e(1509)*/
    ~ClassMemFunGuardWith2RefArg() throw() 
    {
        try 
        {
            if (control_) 
            {
                (obj_.*fun_)(arg1_, arg2_);
            }
        }
        catch(...) 
        {
        }
    }
private:
    T& obj_;
    A& arg1_;
    B& arg2_;
    MemFun fun_;
};

template<class T, typename A, typename B, typename R = void>
class ClassMemFunGuardWith2ConstRefArg : public OperationGuard
{
    typedef R (T::*MemFun)(const A&, const B&);
public:
    ClassMemFunGuardWith2ConstRefArg(T& obj, MemFun _fun, const A& _arg1, const B& _arg2)
        : obj_(obj), arg1_(_arg1), arg2_(_arg2), fun_(_fun)
    {
    }

    /*lint --e(1509)*/
    ~ClassMemFunGuardWith2ConstRefArg() throw() 
    {
        try 
        {
            if (control_) 
            {
                (obj_.*fun_)(arg1_, arg2_);
            }
        }
        catch(...) 
        {
        }
    }
private:
    T& obj_;
    const A& arg1_;
    const B& arg2_;
    MemFun fun_;
};
template<class T, typename R = void>
class ClassMemFunGuardWithNoArg : public OperationGuard
{
    typedef R (T::*MemFun)(void);
public:
    ClassMemFunGuardWithNoArg(T& obj, MemFun _fun)
        : obj_(obj),fun_(_fun)
    {
    }

    /*lint --e(1509)*/
    ~ClassMemFunGuardWithNoArg() throw() 
    {
        try 
        {
            if (control_) 
            {
                (obj_.*fun_)();
            }
        }
        catch(...) 
        {
        }
    }
private:
    T& obj_;
    MemFun fun_;
};

template<class T, typename R = void>
class ConstClassConstMemFunGuardNoArg : public UtilsExt::OperationGuard
{
    typedef R (T::*MemFun)(void) const;
public:
    ConstClassConstMemFunGuardNoArg(const T& obj, MemFun _fun)
        : obj_(obj), fun_(_fun)
    {
    }

    /*lint --e(1509)*/
    ~ConstClassConstMemFunGuardNoArg() throw() 
    {
        try 
        {
            if (control_) 
            {
                (obj_.*fun_)();
            }
        }
        catch(...) 
        {
        }
    }
private:
    const T& obj_;
    MemFun fun_;
};

template<class T, typename R = void>
class ClassMemFunGuardNoArg : public UtilsExt::OperationGuard
{
    typedef R (T::*MemFun)(void) ;
public:
    ClassMemFunGuardNoArg(T& obj, MemFun _fun)
        : obj_(obj), fun_(_fun)
    {
    }

    /*lint --e(1509)*/
    ~ClassMemFunGuardNoArg() throw() 
    {
        try 
        {
            if (control_) 
            {
                (obj_.*fun_)();
            }
        }
        catch(...) 
        {
        }
    }
private:
    T& obj_;
    MemFun fun_;
};


}

#endif //_CME_UTILS_EXT_FUNC_GUARD_MAP_HEADER_
