#pragma once
#pragma once
#ifndef __CONFIG_H__
#define __CONFIG_H__
#include<iostream>
#include<string>
#include<memory>
#include<sstream>//序列化
#include <boost/lexical_cast.hpp>
#include"Log.h"
using namespace std;
//配置系统yaml
//公用属性
class ConfigVarBase {
public:
	typedef shared_ptr<ConfigVarBase> ptr;
	ConfigVarBase(string name, string descripation = "") :
		m_name(name), m_description(descripation) {}
	virtual ~ConfigVarBase() {}
	string getname() { return m_name; };
	string getdescription() { return m_description; }

	virtual string Tostring() = 0;
	virtual bool fromstring(string val) = 0;

protected:
	string m_name;
	string m_description;


};
template<class T>
class ConfigVar :public ConfigVarBase {
public:
	typedef shared_ptr<ConfigVar> ptr;
	ConfigVar(string name, T& val, string description = "")
		: ConfigVarBase(name, description), m_val(val)
	{
	}
	string Tostring() {
		try {
			using boost::lexical_cast;
			//将T 转换为流式string
			return lexical_cast<string> (m_val);//将m_val流式处理返回到标准库中字符串的流中，
			//然后作为模板对象输出，这里模板对象就是string（就是将m_val转换为val，存储在string 然后以string形式返回）

		}
		catch (exception& e) {
			/*
			*e.what() 将异常信息打印出来
			*typeid().name()，可以用来返回变量的数据类型
			*
			*/
			LOG_ERROR(LOG_ROOT()) << "ConfigVal::ToString expection" << e.what() << "convert:" << typeid(m_val).name() << "to string";


		}

	}
	bool fromstring(string val) override {
		try {
			using boost::lexical_cast;
			//就是将string类型转换为模板T
			return m_val = lexical_cast<T>(val);//将m_val流式处理返回到标准库中字符串的流中，
			//然后作为模板对象输出，这里模板对象就是T（就是将m_val转换为val，存储在string 然后以T形式返回）

		}
		catch (exception& e) {
			/*
			*e.what() 将异常信息打印出来
			*typeid().name()，可以用来返回变量的数据类型
			*
			*/
			LOG_ERROR(LOG_ROOT()) << "ConfigVal::ToString expection" << e.what() << "convert: string to" << typeid(m_val).name();


		}


	}
	T getval() { return m_val; }
	void setval(T val) { m_val = val; }

private:
	T m_val;

};


//配置管理器

class Config {
public:
	typedef map<string, ConfigVarBase::ptr> ConfigValMap;
	static ConfigValMap cvm;


public:

	template<class T>
	static   typename ConfigVar<T>::ptr LookUp(string name, T val, string description = "") {
		auto tmp = LookUp<T>(name);
		if (tmp) {
			LOG_INFO(LOG_ROOT()) << "LookUp name " << name << " exists";
			return tmp;
		}
		if (name.find_first_not_of("abcdefghijklmnopqistuvwxyzABCDEFGHIJKLMNOPQRSTUVWSYZ._12345678") != string::npos) {
			//正向查找在原字符串中第一个与指定字符串（或字符）中的任一字符都不匹配的字符，返回它的位置。若查找失败，则返回npos。
			LOG_ERROR(LOG_ROOT()) << "LookUp name invalid " << name;
			//	throw invalid_argument(name);//抛出无效参数name
		}
		typename ConfigVar<T>::ptr v(new ConfigVar<T>(name, val, description));
		cvm[name] = v;
		return v;

	}
	template<class T>
	static typename   ConfigVar<T>::ptr LookUp(string name) {
		auto it = cvm.find(name);
		//没有找到
		if (it == cvm.end()) {
			return nullptr;
		}
		//智能指针的向上转换  派生类-->基类
		//没有智能指针时可用dynamic_cast转换
		return dynamic_pointer_cast<ConfigVar<T>> (it->second);

	}


};
#endif // !__CONIFIG_H__
