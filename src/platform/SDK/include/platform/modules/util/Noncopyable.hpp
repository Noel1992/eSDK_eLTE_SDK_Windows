#ifndef _CME_UTILS_EXT_NONCOPYABLE_H_INCLUDED_
#define _CME_UTILS_EXT_NONCOPYABLE_H_INCLUDED_

namespace UtilsExt {

/*lint -esym(1509, Noncopyable)*/
/*lint -esym(1932, Noncopyable)*/ //屏蔽Noncopyable不是虚基类的错误
class Noncopyable {
protected:
	Noncopyable(){}
	~Noncopyable() throw() {}
private:  // emphasize the following members are private
	Noncopyable( const Noncopyable& );
	const Noncopyable& operator=( const Noncopyable& );
}; // Noncopyable

/*lint -esym(1509, NoncopyableT)*/
/*lint -esym(1932, *NoncopyableT*)*/ //屏蔽NoncopyableT不是虚基类的错误
/*lint -esym(1928, *NoncopyableT*)*/ //屏蔽NoncopyableT没有在初始化列表中的错误
template<typename T>
class NoncopyableT {
protected:
	NoncopyableT(){}
	~NoncopyableT() throw() {}
private:  // emphasize the following members are private
	NoncopyableT( const NoncopyableT& );
	const NoncopyableT& operator=( const NoncopyableT& );
}; // Noncopyable

}

#endif//_CME_UTILS_EXT_NONCOPYABLE_H_INCLUDED_
