#pragma once
#pragma once
#ifndef __CONFIG_H__
#define __CONFIG_H__
#include<iostream>
#include<string>
#include<memory>
#include<sstream>//���л�
#include <boost/lexical_cast.hpp>
#include"Log.h"
using namespace std;
//����ϵͳyaml
//��������
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
			//��T ת��Ϊ��ʽstring
			return lexical_cast<string> (m_val);//��m_val��ʽ�����ص���׼�����ַ��������У�
			//Ȼ����Ϊģ��������������ģ��������string�����ǽ�m_valת��Ϊval���洢��string Ȼ����string��ʽ���أ�

		}
		catch (exception& e) {
			/*
			*e.what() ���쳣��Ϣ��ӡ����
			*typeid().name()�������������ر�������������
			*
			*/
			LOG_ERROR(LOG_ROOT()) << "ConfigVal::ToString expection" << e.what() << "convert:" << typeid(m_val).name() << "to string";


		}

	}
	bool fromstring(string val) override {
		try {
			using boost::lexical_cast;
			//���ǽ�string����ת��Ϊģ��T
			return m_val = lexical_cast<T>(val);//��m_val��ʽ�����ص���׼�����ַ��������У�
			//Ȼ����Ϊģ��������������ģ��������T�����ǽ�m_valת��Ϊval���洢��string Ȼ����T��ʽ���أ�

		}
		catch (exception& e) {
			/*
			*e.what() ���쳣��Ϣ��ӡ����
			*typeid().name()�������������ر�������������
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


//���ù�����

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
			//���������ԭ�ַ����е�һ����ָ���ַ��������ַ����е���һ�ַ�����ƥ����ַ�����������λ�á�������ʧ�ܣ��򷵻�npos��
			LOG_ERROR(LOG_ROOT()) << "LookUp name invalid " << name;
			//	throw invalid_argument(name);//�׳���Ч����name
		}
		typename ConfigVar<T>::ptr v(new ConfigVar<T>(name, val, description));
		cvm[name] = v;
		return v;

	}
	template<class T>
	static typename   ConfigVar<T>::ptr LookUp(string name) {
		auto it = cvm.find(name);
		//û���ҵ�
		if (it == cvm.end()) {
			return nullptr;
		}
		//����ָ�������ת��  ������-->����
		//û������ָ��ʱ����dynamic_castת��
		return dynamic_pointer_cast<ConfigVar<T>> (it->second);

	}


};
#endif // !__CONIFIG_H__
