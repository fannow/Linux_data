#include"Log.hpp"
#include"Config.h"


//����ϵͳ����־ϵͳ����
struct LogAppenderDefine {
    int type = 0;//��־�����  1  ������ļ� 2 ���������̨
    LogLevel::Level level = LogLevel::UNKNOW;
    string formater;//��־�����ʽ
    string file;
    bool operator==(const LogAppenderDefine& l)const {

        return type == l.type && level == l.level &&
            formater == l.formater && file == l.file;


    }
};


struct LogDefine {
    string name;
    LogLevel::Level level = LogLevel::UNKNOW;
    string formater;


    vector<LogAppenderDefine> addernders;

    bool operator==(const LogDefine& l)const {
        return name == l.name && level == l.level && formater == l.formater && addernders == l.addernders;
    }
    bool operator <(const LogDefine& l) const{
        return  name < l.name;

    }
};
//��־ϵͳ��ƫ�ػ�
template<>
class LexicalCast<string,set<LogDefine>>{
  public:
   set<LogDefine> operator()(string str){
     stringstream ss;
      YAML::Node root=YAML::Load(str);//����ת��Ϊ����
     typename   std::set<LogDefine> vec;
      for(int i=0;i<root.size();i++){
          auto it=root[i];
          if(!it["name"].IsDefined()){
            cout<<"log config error: name is null"<<it<<endl;
            continue;
          }
        LogDefine id;
        id.name=it["name"].as<string>();
        id.level=LogLevel::Formstring(it["level"].IsDefined()?it["level"].as<string>(): "");
        if(it["formater"].IsDefined()){
          id.formater=it["formater"].as<string>();

        }
        if(it["addernders"].IsDefined()){
          for(int x=0;x<it["addernders"].size();x++){
            auto a=it["addernders"][x];
            LogAppenderDefine lad;
            if(!a["type"].IsDefined()){
            cout<<"log config error: appender is null"<<a<<endl;
              continue;
            }
            string type =a["type"].as<string>();
            cout<<"type"<<type<<endl;
            if(type=="FileAppender"){
                lad.type=1;
                //��־�ļ�·��
                if(!a["file"].IsDefined()){
                          
                cout<<"log config error: file  is invaild"<<endl;
                continue;
                }
                lad.file=a["file"].as<string>();
              if(a["formater"].IsDefined()){
                lad.formater=a["formater"].as<string>();
               }
            }else if(type=="StdOutAppender"){
                lad.type=2;
            }else{
            cout<<"log config error: addernder type is invaild "<<type<<endl;
              continue;
            }
            id.addernders.push_back(lad);
          }       
        }
         vec.insert(id);

      }
      return vec;
    }
     
};
template<>
class LexicalCast<set<LogDefine>,string>{
  public:
   string operator()(set<LogDefine> &v){
     stringstream ss;
     YAML::Node root;
      for(auto &o:v){
      YAML::Node n;
      n["name"]=o.name;
      n["level"]=LogLevel::Tostring(o.level);
      if(!o.formater.empty()){
        n["formater"]=o.formater;
      }
      for(auto &i:o.addernders){
        YAML::Node a;
        if(i.type==1){
            a["type"]="FileAppender";
            a["file"]=i.file;
        }else if(i.type==2){
          a["type"]="StdOutAppender";
        }
        a["level"]=LogLevel::Tostring(i.level);   
         if(!i.formater.empty()){
          a["formater"]=i.formater;
         }
        n["addernders"].push_back(n);
       }
      root.push_back(n);
      }
      ss<<root;
      return ss.str();
   }
};
ConfigVar<set<LogDefine>>::ptr logdefine= Config::LookUp("logs", set<LogDefine>(), "logs config");
struct LogIniner {
    LogIniner() {
        logdefine->AddListener(0xABCDEF, [](const set<LogDefine>& old, const set<LogDefine>& nwe) {
            cout << "logger_change" << endl;
            //����
            for (auto& o : nwe) {
                Logger::ptr logger;
                auto it = old.find(o);
                if (it == old.end()) {
                cout<<"���"<<endl;
                    //���ϵ���Ѱ�����û���ҵ�����ӵ�����
                    logger.reset(new Logger(o.name));            
                }
                else {
                    if (!(o == *it)) {
                    cout<<"����"<<endl;
                        //�޸�logger
                        logger= LOG_NAME(o.name);//���ҵ���Ӧlogger
                    }
          
                }
                logger->setleve(o.level);
                if (!o.formater.empty()) {
                    logger->SetFomater(o.formater);     
                }

                logger->ClearAppender();
                for (auto& i : o.addernders) {
                    LogAppender::ptr ap;
                    if (i.type == 1) {
                        //����ļ����
                        ap.reset(new FileAppender(i.file));
                    }
                    else if (i.type == 2) {
                        //��ӿ���̨���
                        ap.reset(new StdOutAppender());
                    }
                    ap->setlevel(i.level);
                    logger->AddAppender(ap);

            }
                        //ɾ��  ------���µ����Ҳ����ϵľ�ɾ��
                        for (auto& i : old) {
                            auto it = nwe.find(i);
                            if (it == nwe.end()) {
                                auto logger = LOG_NAME(i.name);
                                logger->setleve((LogLevel::Level)100);
                                logger->ClearAppender();
                          }
                      }

                  }
        });
      }
            
   };//��־��ϵͳ��ʼ��
struct LogIniner _init_log;
