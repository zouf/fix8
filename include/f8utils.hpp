//-----------------------------------------------------------------------------------------
#if 0

Orbweb is released under the New BSD License.

Copyright (c) 2007-2010, David L. Dight <www@orbweb.org>
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are
permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this list of
	 	conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list
	 	of conditions and the following disclaimer in the documentation and/or other
		materials provided with the distribution.
    * Neither the name of the author nor the names of its contributors may be used to
	 	endorse or promote products derived from this software without specific prior
		written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS
OR  IMPLIED  WARRANTIES,  INCLUDING,  BUT  NOT  LIMITED  TO ,  THE  IMPLIED  WARRANTIES  OF
MERCHANTABILITY AND  FITNESS FOR A PARTICULAR  PURPOSE ARE  DISCLAIMED. IN  NO EVENT  SHALL
THE  COPYRIGHT  OWNER OR  CONTRIBUTORS BE  LIABLE  FOR  ANY DIRECT,  INDIRECT,  INCIDENTAL,
SPECIAL,  EXEMPLARY, OR CONSEQUENTIAL  DAMAGES (INCLUDING,  BUT NOT LIMITED TO, PROCUREMENT
OF SUBSTITUTE  GOODS OR SERVICES; LOSS OF USE, DATA,  OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED  AND ON ANY THEORY OF LIABILITY, WHETHER  IN CONTRACT, STRICT  LIABILITY, OR
TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

$Id: xml.hpp 497 2010-01-10 01:57:03Z davidd $
$LastChangedDate: 2010-01-10 12:57:03 +1100 (Sun, 10 Jan 2010) $
$Rev: 497 $
$URL: svn://catfarm.electro.mine.nu/usr/local/repos/mongod/trunk/base/xml.hpp $

#endif
//-----------------------------------------------------------------------------------------
#ifndef _F8_UTILS_HPP_
#define _F8_UTILS_HPP_

namespace FIX {

//----------------------------------------------------------------------------------------
std::string& InPlaceChrReplace(const char sch, const char rch, std::string& source);
std::string& InPlaceStrToUpper(std::string& src);
std::string& InPlaceStrToLower(std::string& src);
const std::string& StrToLower(const std::string& src, std::string& target);
unsigned ROT13Hash (const std::string& str);
std::string& CheckAddTrailingSlash(std::string& source);
std::string& CheckRemoveTrailingSlash(std::string& src);

//----------------------------------------------------------------------------------------
// case insensitive ==
template<typename _CharT, typename _Traits, typename _Alloc>
	inline bool operator% (const std::basic_string<_CharT, _Traits, _Alloc>& __lhs,
		const std::basic_string<_CharT, _Traits, _Alloc>& __rhs)
			{ return strcasecmp(__lhs.c_str(), __rhs.c_str()) == 0; }

template<typename _CharT, typename _Traits, typename _Alloc>
	inline bool operator% (const _CharT* __lhs, const std::basic_string<_CharT, _Traits, _Alloc>& __rhs)
		{ return strcasecmp(__lhs, __rhs.c_str()) == 0; }

template<typename _CharT, typename _Traits, typename _Alloc>
	inline bool operator% (const std::basic_string<_CharT, _Traits, _Alloc>& __lhs, const _CharT* __rhs)
		{ return strcasecmp(__lhs.c_str(), __rhs) == 0; }

// case insensitive <
template<typename _CharT, typename _Traits, typename _Alloc>
	inline bool operator^ (const std::basic_string<_CharT, _Traits, _Alloc>& __lhs,
		const std::basic_string<_CharT, _Traits, _Alloc>& __rhs)
			{ return strcasecmp(__lhs.c_str(), __rhs.c_str()) < 0; }

//----------------------------------------------------------------------------------------
template <typename T>
class scoped_ptr
{
	T *ptr_;

	scoped_ptr(const scoped_ptr&);
	void operator=(const scoped_ptr&);

public:
	explicit scoped_ptr(T *p=0) : ptr_(p) {}
	~scoped_ptr() { delete ptr_; }

	template <typename U>
	bool operator==(const scoped_ptr<U>& that) const { return ptr_ == that.get(); }
	template <typename U>
	bool operator==(const scoped_ptr<U> *that) const { return ptr_ == that; }

	template <typename U>
	bool operator!=(const scoped_ptr<U>& that) const { return ptr_ != that.get(); }
	template <typename U>
	bool operator!=(const scoped_ptr<U> *that) const { return ptr_ != that; }

	bool operator==(const scoped_ptr<T>& that) const { return ptr_ == that.get(); }
	bool operator==(const T *that) const { return (ptr_ == that); }

	bool operator!=(const scoped_ptr<T>& that) const { return ptr_ != that.get(); }
	bool operator!=(const T *that) const { return ptr_ != that; }

	T *operator->() const { return ptr_; }
	T& operator*() const { return *ptr_; }
	T *release() { T *tmp(ptr_); ptr_ = 0; return tmp; }
	void reset(T *p=0) { delete ptr_; ptr_ = p; }
	T *get() const { return ptr_; }
};

//----------------------------------------------------------------------------------------
class RegExp
{
	static const int SubLimit_ = 32, MaxErrLen_ = 256;

	regex_t reg_;
	regmatch_t subexprs_[SubLimit_];
	const std::string pattern_;
	std::string errString;
	int subCnt_, errCode_;

public:
	RegExp(const char *pattern, const int flags=0);
	virtual ~RegExp() { if (errCode_ == 0) regfree(&reg_); }

	int SearchString (const std::string& source, const int subExpr=SubLimit_);
	const int SubCnt() const { return subCnt_; }
	std::string& SubExpr (const std::string& source, std::string& target, const int num=0);
	std::string& Erase(std::string& source, const int num=0);
	std::string& Replace(std::string& source, const std::string& with, const int num=0);

	const std::string& GetPattern() const { return pattern_; }
	const std::string& ErrString() const { return errString; }
	bool operator!() const { return errCode_; }
	operator void*() { return errCode_ ? 0 : this; }
};

//----------------------------------------------------------------------------------------
struct StringPtrLessThanNoCase
{
	bool operator()(const std::string *a, const std::string *b) const
		{ return *a ^ *b; }
};

struct StringPtrLessThan
{
	bool operator()(const std::string *a, const std::string *b) const
		{ return *a < *b; }
};

struct StringLessThanNoCase
{
	bool operator()(const std::string& a, const std::string& b) const
		{ return a ^ b; }
};

//----------------------------------------------------------------------------------------
template<typename T>
inline T GetValue(const std::string& source, T defval)
{
	if (source.empty())
		return defval;
	std::istringstream istr(source);
	T result(defval);
	istr >> result;
	return result;
}

template<typename T>
inline T GetValue(const std::string& source)
{
	std::istringstream istr(source);
	T result;
	istr >> result;
	return result;
}

template<>
inline bool GetValue(const std::string& source)
{
	if (source.empty())
		return false;
#if !defined XMLENTITY_STRICT_BOOL
	return source % "true" || source % "yes" || source % "y" || source == "1";
#else
	bool result(false);
	std::istringstream istr(source);
	istr >> std::boolalpha >> result;
	return result;
#endif
}

template<typename T>
inline const std::string& PutValue(const T& a, std::string& target)
{
	std::ostringstream ostr;
	ostr << a;
	return target = ostr.str();
}

//----------------------------------------------------------------------------------------
template<typename T>
inline T *Alloca(const size_t sz)
{
	return static_cast<T *>(alloca(sz * sizeof(T)));
}

//----------------------------------------------------------------------------------------
// bitset for enums
template<typename T, typename B=unsigned int>
class ebitset
{
	typedef B integral_type;
	integral_type a_;

public:
	explicit ebitset() : a_() {}
	explicit ebitset(const integral_type a) : a_(a) {}
	explicit ebitset(T sbit) : a_((1 << sbit) - 1) {}

	ebitset<T, B>& operator=(const ebitset<T, B>& that)
	{
		if (this != &that)
			a_ = that.a_;
		return *this;
	}

	bool has(const T sbit) { return a_ & 1 << sbit; }
	void set(const T sbit) { a_ |= 1 << sbit; }
	void set(const integral_type bset) { a_ = bset; }
	void clear(const T sbit) { a_ &= ~(1 << sbit); }
	void clearall() { a_ = 0; }
	void setall(const T sbit) { a_ = (1 << sbit) - 1; }
	integral_type get() const { return a_; }
};

//----------------------------------------------------------------------------------------
inline char *CopyString(const std::string& src, char *target, unsigned limit=0)
{
	if (!target)
		return 0;
	unsigned sz(limit && src.size() > limit ? limit : src.size() + 1);
	src.copy(target, sz - 1);
	target[sz - 1] = 0;
	return target;
}

//-----------------------------------------------------------------------------------------
inline char *CopyAlloc (const std::string& what)
{
	return what.empty() ? 0 : CopyString(what, new char[what.size() + 1]);
}

//----------------------------------------------------------------------------------------
template<typename T>
inline T rotl(const T val, const int bits)
{
	return val << bits | val >> (sizeof(T) * 8 - bits);
}

//----------------------------------------------------------------------------------------
inline bool exist(const std::string& fname)
{
	return access(fname.c_str(), F_OK) == 0;
}

//----------------------------------------------------------------------------------------
inline int millisleep (const int ms)
{
	struct timespec tspec = { ms / 1000, 1000 * 1000 * (ms % 1000) };
	return nanosleep(&tspec, 0);
}

//----------------------------------------------------------------------------------------
struct DeleteObject
{
	template<typename T>
	void operator()(const T& m) const { delete m; }
};

struct DeleteArrayObject
{
	template<typename T>
	void operator()(const T& m) const { delete[] m; }
};

template <class Deleter = DeleteObject>
struct Delete1stPairObject
{
	template<typename A, typename B>
	void operator()(const std::pair<A, B>& m) const { Deleter()(m.first); }
};

template <class Deleter = DeleteObject>
struct Delete2ndPairObject
{
	template<typename A, typename B>
	void operator()(const std::pair<A, B>& m) const { Deleter()(m.second); }
};

template <class Deleter = DeleteObject>
struct free_ptr
{
	template<typename T>
	void operator()(const T& ptr) const { Deleter()(ptr); }
};

} // namespace FIX

#endif // _F8_UTILS_HPP_
