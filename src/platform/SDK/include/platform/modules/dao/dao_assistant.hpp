/********************************************************************
Copyright 2012,TD-Tech. Co., Ltd.
Filename:      dao_assistant.hpp
Author:        XiaoLongguang
Created:       2012/10/29 09:17
Description:
*********************************************************************/

#ifndef UBP_PLATFORM_MODULES_DAO_DAO_ASSISTANT_HPP_
#define UBP_PLATFORM_MODULES_DAO_DAO_ASSISTANT_HPP_

#include <string>
#include <vector>
#include <map>
#include <odb/database.hxx>
#include <odb/traits.hxx>

#include "platform/modules/frame/module.hpp"
#include "platform/modules/frame/module_context.hpp"
#include "dao_common.hpp"
#include "dao_error.hpp"
#include "platform/modules/encryption/encryption.hpp"

#include <odb/exceptions.hxx>

namespace ubp
{
namespace platform
{
namespace dao
{

/* �������ֶ��������ֶ����� */
/*typedef struct column_type_s
{
  std::string s_name;   //���ֶ���
  int i_value;          //�ֶ����ԣ�����
  std::string s_value;  //�ֶ����ԣ����ζ�Ӧ���ַ���
} column_type;
*/
/* �������ֶ��������ֶ�����vector��˳���д����� */
//typedef std::vector<column_type> vec_column_name;


/* ��������vector��Ԫ��Ϊmap��һ��mapΪһ�����ݣ�map��Ԫ��Ϊ������ֵ��ֵΪstring�����Լ�ת�� */
typedef std::vector<std::map<std::string, std::string> > vec_table_data;
typedef std::vector<std::vector<std::string> > vec_raw_table_data;

typedef std::vector<std::string> SQLStrVec;

typedef odb::connection Connection_type;
typedef odb::database Database_type;

class DAO_IMPORT_EXPORT DAOInfo
{
  public:
    DAOInfo(const std::string &user,
            const std::string &passwd,
            const std::string &database,
            const std::string &host,
            const std::string &port,
            unsigned int max_connection,
            unsigned int min_connection,
            odb::database *pDb);
    virtual ~DAOInfo();

  public:
    std::string dbUserName_;
    std::string dbPwd_;
    std::string dbName_;
    std::string dbHost_;
    std::string dbPort_;
    ACE_INT32 maxCnt_;
    ACE_INT32 minCnt_;
    odb::database *pDataBase_;
};


class DaoAssistant;
class DAO_IMPORT_EXPORT DBTransaction 
{

friend class DaoAssistant;

public:
  
    explicit DBTransaction(DAOInfo& daoinfo);

    ~DBTransaction(void);

public:

    //�����������������
    void Reset(DAOInfo& daoinfo);
    

    //�ύ
    bool Commit();
    
    //�ع�
    bool Rollback();

    //�Ƿ��Ѿ��ύ��ع���
    bool Status();
      

private:

    // Copying or assignment of transactions is not supported.

    DBTransaction (const DBTransaction&);

    DBTransaction& operator= (const DBTransaction&);

    Connection_type& Connection ();


protected:

    bool finalized_;
    
    std::auto_ptr<odb::transaction_impl> impl_;

    //Connection_type&  conn_;
    
   
};



class DAO_IMPORT_EXPORT DaoAssistant
  : public ubp::platform::frame::Module
{

  public:
    //Module must support a static function "ModuleName"
    static const std::string &ModuleName(void);

    static DaoAssistant *CreateInstance();

    /********************************************************************
    create dao instance
    ********************************************************************/
    static DAOInfo* CreateDao(const std::string &user,
                                    const std::string &passwd,
                                    const std::string &database,
                                    const std::string &host,
                                    const std::string &port,
                                    unsigned int max_connection,
                                    unsigned int min_connection);

    virtual ACE_INT32 Init(void) = 0;
	virtual ACE_INT32 Init( ubp::platform::encryption::Encryption * pEncryption ) = 0;

    virtual DAOInfo *GetDAO(const std::string &moc = "") = 0;


    /********************************************************************
    Method:      Init
    FullName:    DaoAssistant::Init
    Access:      public
    Returns:     0�ɹ�����0ʧ��
    Description: �������ݿ⣬�������ӳز������������ֵ����Сֵ
    Parameter:   user�û�����passwd���룬database������host������port�˿ں�
    Parameter:   max_connection���ӳ���󲢷���������ﵽ�����µ�����������
                 �����ȴ����������ͷţ������Ϊ0����������ʱ�ʹ����µ�
    Parameter:   min_connection���ӳ���С������������������ӳ�����Сֵ����
                 �ر����ӣ������Ϊ0���������ͷ�ʱ���ر�
    ********************************************************************/
    //virtual ACE_INT32 Init(const std::string &user,
    //                       const std::string &passwd,
    //                       const std::string &database,
    //                       const std::string &host,
    //                       const std::string &port,
    //                       unsigned int max_connection = 0,
    //                       unsigned int min_connection = 5) = 0;


    /********************************************************************
    Method:      ExecuteNativeSql
    FullName:    DaoAssistant::ExecuteNativeSql
    Access:      public
    Returns:     >=0 �ɹ���ֵΪsqlӰ��Ľ��������<0 ʧ��
    Description: ִ��ԭʼSQL��䣬ʹ��ODB����Ҫ������ɾ�Ĳ�������ѯ�������������ݣ�������
    Parameter:   sqlԭʼSQL���
    ********************************************************************/
    virtual ACE_INT32 ExecuteNativeSql(const std::string &sql, DAOInfo *pDao=NULL) = 0;


    virtual ACE_INT32 ExecuteNativeSql(const std::string &sql, DBTransaction& trans) = 0;


    /********************************************************************
    Method:      ExecuteNativeSelCnt
    FullName:    DaoAssistant::ExecuteNativeSelCnt
    Access:      public
    Returns:     >=0 �ɹ���ֵΪselect count��ֵ��<0 ʧ��
    Description: ִ��ԭʼSQL��ѯ���(select count)��ֱ��ʹ��MySQL�ӿڣ����ؽ������
    Parameter:   selectԭʼSQL��ѯ���(select count)
    ********************************************************************/
    virtual ACE_INT32 ExecuteNativeSelCnt(const char *select, DAOInfo *pDao=NULL) = 0;


    virtual ACE_INT32 ExecuteNativeSelCnt(const char *select, DBTransaction& trans) = 0;


    /********************************************************************
    Method:      ExecuteNativeSelect
    FullName:    DaoAssistant::ExecuteNativeSelect
    Access:      public
    Returns:     0�ɹ���1��ѯ�޽������ֵʧ��
    Description: ִ��ԭʼSQL��ѯ��䣬ֱ��ʹ��MySQL�ӿڣ����ز�ѯ��������ֶ���������
    Parameter:   records��ѯ�����vector��һ��Ԫ��mapΪһ�����ݣ�map��Ԫ��Ϊ������ֵ��ֵΪstring�����Լ�ת��
                 sqlԭʼSQL��ѯ���
    ********************************************************************/
    virtual ACE_INT32 ExecuteNativeSelect(vec_table_data &records,
                                          const char *select,
                                          DAOInfo *pDao=NULL) = 0;

    /********************************************************************
    Method:      ExecuteNativeSelect
    FullName:    DaoAssistant::ExecuteNativeSelect
    Access:      public
    Returns:     0�ɹ���1��ѯ�޽������ֵʧ��
    Description: ִ��ԭʼSQL��ѯ��䣬ֱ��ʹ��MySQL�ӿڣ����ز�ѯ��������ֶ���������
    Parameter:   records��ѯ�����vector��һ��Ԫ��Ϊһ������,��һ��������,ʣ�µ���ÿ�е�ֵ
    ********************************************************************/
    virtual ACE_INT32 ExecuteNativeSelect(vec_raw_table_data &records,
                                          const char *select,
                                          DAOInfo *pDao=NULL) = 0;

    virtual ACE_INT32 ExecuteNativeSelect(vec_table_data &records,
                                          const char *select,
                                          DBTransaction& trans)= 0;
    /*int ExecuteNativeSelect(vec_table_data &records,
        vec_column_name &column_name,
        const char *select);
    */


    /********************************************************************
    Method:      ExecuteNativeBatchSql
    FullName:    DaoAssistant::ExecuteNativeBatchSql
    Access:      public
    Returns:     >=0 �ɹ�,Ӱ����������<0 ʧ��
    Description: ����SQL�ӿڣ�ִ��ԭʼSQL��䣬ֱ��ʹ��MySQL�ӿڣ�>0���ؽ��������<0������
    Parameter:   vector,ԭʼSQL���
    ********************************************************************/
    virtual ACE_INT32 ExecuteNativeBatchSql(const SQLStrVec &batch_sql,
        DAOInfo *pDao=NULL) = 0;


    /********************************************************************
    Method:      Find
    FullName:    DaoAssistant::Find
    Access:      public
    Returns:     0�ɹ�����0ʧ��
    Description: ���Ҷ�����������ݣ��޲�ѯ����
    Parameter:   �����ѯ�����vector
    ********************************************************************/
    template<typename T>
    ACE_INT32 Find(std::vector<T> &records);


    /********************************************************************
    Method:      Find
    FullName:    DaoAssistant::Find
    Access:      public
    Returns:     0�ɹ�����0ʧ��
    Description: ���ݲ�ѯ�������Ҷ���������
    Parameter:   records�����ѯ�����vector��query_cond�����Ͳ�ѯ����
    ********************************************************************/
    template<typename T>
    ACE_INT32 Find(std::vector<T> &records, odb::query<T> query_cond);


    /********************************************************************
    Method:      Find
    FullName:    DaoAssistant::Find
    Access:      public
    Returns:     0�ɹ�����0ʧ��
    Description: ���ݲ�ѯ�������Ҷ���������
    Parameter:   records�����ѯ�����vector��query_condΪSQL���Ĳ�ѯ����
    ********************************************************************/
    template<typename T>
    ACE_INT32 Find(std::vector<T> &records, const std::string &query_cond);


    /********************************************************************
    Method:      Insert
    FullName:    DaoAssistant::Insert
    Access:      public
    Returns:     0�ɹ�����0ʧ��
    Description: ���뵥������
    Parameter:   objΪ��������
    ********************************************************************/
    template<typename T>
    ACE_INT32 Insert(T &obj);


    /********************************************************************
    Method:      Insert
    FullName:    DaoAssistant::Insert
    Access:      public
    Returns:     0�ɹ�����0ʧ��
    Description: ����������ÿ1000���ύһ��
    Parameter:   objΪ��������vector
    ********************************************************************/
    template<typename T>
    ACE_INT32 Insert(std::vector<T> &objs);


    /********************************************************************
    Method:      Delete
    FullName:    DaoAssistant::Delete
    Access:      public
    Returns:     0�ɹ�����0ʧ��
    Description: ɾ����������
    Parameter:   objΪ��������
    ********************************************************************/
    template<typename T>
    ACE_INT32 Delete(T &obj);


    /********************************************************************
    Method:      Delete
    FullName:    DaoAssistant::Delete
    Access:      public
    Returns:     0�ɹ�����0ʧ��
    Description: ɾ���������
    Parameter:   objΪ�����������vector
    ********************************************************************/
    template<typename T>
    ACE_INT32 Delete(std::vector<T> &objs);


    /********************************************************************
    Method:      Update
    FullName:    DaoAssistant::Update
    Access:      public
    Returns:     0�ɹ�����0ʧ��
    Description: ���µ�������,����ǰ��ʹ��Find��������ҳ���
    Parameter:   objΪ��������
    ********************************************************************/
    template<typename T>
    ACE_INT32 Update(T &obj);


    /********************************************************************
    Method:      Update
    FullName:    DaoAssistant::Update
    Access:      public
    Returns:     0�ɹ�����0ʧ��
    Description: ���¶������,����ǰ��ʹ��Find��������ҳ���
    Parameter:   objΪ�����������vector
    ********************************************************************/
    template<typename T>
    ACE_INT32 Update(std::vector<T> &objs);

  protected:

    Connection_type& getConnection(DBTransaction& trans);

  private:
    //��ȡ��ǰ���ݿ��ָ��
   virtual odb::database *GetDb(const std::string &moc = "") = 0;

   

};

inline Connection_type& DaoAssistant::getConnection(DBTransaction& trans)
{
    return trans.Connection ();
}


template<typename T>
ACE_INT32 DaoAssistant::Find(std::vector<T> &records)
{
  typedef odb::query<T> query;
  return Find<T>(records, query::true_expr);
}

template<typename T>
ACE_INT32 DaoAssistant::Find(std::vector<T> &records, odb::query<T> query_cond)
{
  try
  {
    typedef typename odb::result<T> result;
    odb::transaction t(GetDb()->begin());
    result r(GetDb()->query<T> (query_cond));
    typename result::iterator i(r.begin());
    for(; i != r.end(); ++i)
    {
      records.push_back(*i);
    }
    t.commit();
    return UBP_SUCCESS;
  }
  catch(const odb::database_exception &e)
  {
    UBP_DAO_ERROR("DaoAssistant::Find query failed: " << e.what());
    return UBP_FAIL;
  }
  catch(const odb::exception &e)
  {
    //std::cerr << e.what() << std::endl;
    UBP_DAO_ERROR("DaoAssistant::Find query error: " << e.what());
    return UBP_FAIL;
  }
}

template<typename T>
ACE_INT32 DaoAssistant::Find(std::vector<T> &records, const std::string &query_cond)
{
  try
  {
    typedef typename odb::result<T> result;
    odb::transaction t(GetDb()->begin());
    result r(GetDb()->query<T> (query_cond));
    typename result::iterator i(r.begin());
    for(; i != r.end(); ++i)
    {
      records.push_back(*i);
    }
    t.commit();
    return UBP_SUCCESS;
  }
  catch(const odb::database_exception &e)
  {
    UBP_DAO_ERROR("DaoAssistant::Find string failed: " << e.what());
    return UBP_FAIL;
  }
  catch(const odb::exception &e)
  {
    //std::cerr << e.what() << std::endl;
    UBP_DAO_ERROR("DaoAssistant::Find string error: " << e.what());
    return UBP_FAIL;
  }
}

template<typename T>
ACE_INT32 DaoAssistant::Insert(T &obj)
{
  try
  {
    odb::transaction t(GetDb()->begin());
    GetDb()->persist<T>(obj);
    t.commit();
    return UBP_SUCCESS;
  }
  catch(const odb::database_exception &e)
  {
    UBP_DAO_ERROR("DaoAssistant::Insert obj failed: " << e.what());
    return UBP_FAIL;
  }
  catch(const odb::exception &e)
  {
    UBP_DAO_ERROR("DaoAssistant::Insert obj error: " << e.what());
    return UBP_DAO_OBJ_EXIST;
  }
}

template<typename T>
ACE_INT32 DaoAssistant::Insert(std::vector<T> &objs)
{
  try
  {
    odb::transaction t(GetDb()->begin());
    typename std::vector<T>::iterator iter = objs.begin();
    for(ACE_INT32 j = 1; iter != objs.end(); ++iter, ++j)
    {
      GetDb()->persist(*iter);
      if(j % 1000 == 0)
      {
        t.commit();
        t.reset(GetDb()->begin());
      }
    }
    t.commit();
    return UBP_SUCCESS;
  }
  catch(const odb::database_exception &e)
  {
    UBP_DAO_ERROR("DaoAssistant::Insert vec failed: " << e.what());
    return UBP_FAIL;
  }
  catch(const odb::exception &e)
  {
    //std::cerr << e.what() << std::endl;
    UBP_DAO_ERROR("DaoAssistant::Insert vec error: " << e.what());
    return UBP_FAIL;
  }
}

template<typename T>
ACE_INT32 DaoAssistant::Delete(T &obj)
{
  try
  {
    odb::transaction t(GetDb()->begin());
    GetDb()->erase<T>(obj);
    t.commit();
    return UBP_SUCCESS;
  }
  catch(const odb::database_exception &e)
  {
    UBP_DAO_ERROR("DaoAssistant::Delete obj failed: " << e.what());
    return UBP_FAIL;
  }
  catch(const odb::exception &e)
  {
    //std::cerr << e.what() << std::endl;
    UBP_DAO_ERROR("DaoAssistant::Delete obj error: " << e.what());
    return UBP_FAIL;
  }
}

template<typename T>
ACE_INT32 DaoAssistant::Delete(std::vector<T> &objs)
{
  try
  {
    odb::transaction t(GetDb()->begin());
    typename std::vector<T>::iterator iter = objs.begin();
    for(ACE_INT32 j = 1; iter != objs.end() ; ++iter, ++j)
    {
      GetDb()->erase(*iter);
      if(j % 1000 == 0)
      {
        t.commit();
        t.reset(GetDb()->begin());
      }
    }
    t.commit();
    return UBP_SUCCESS;
  }
  catch(const odb::database_exception &e)
  {
    UBP_DAO_ERROR("DaoAssistant::Delete vec failed: " << e.what());
    return UBP_FAIL;
  }
  catch(const odb::exception &e)
  {
    //std::cerr << e.what() << std::endl;
    UBP_DAO_ERROR("DaoAssistant::Delete vec error: " << e.what());
    return UBP_FAIL;
  }
}

template<typename T>
ACE_INT32 DaoAssistant::Update(T &obj)
{
  try
  {
    odb::transaction t(GetDb()->begin());
    GetDb()->update<T>(obj);
    t.commit();
    return UBP_SUCCESS;
  }
  catch(const odb::database_exception &e)
  {
    UBP_DAO_ERROR("DaoAssistant::Update obj failed: " << e.what());
    return UBP_FAIL;
  }
  catch(const odb::exception &e)
  {
    //std::cerr << e.what() << std::endl;
    UBP_DAO_ERROR("DaoAssistant::Update obj error: " << e.what());
    return UBP_FAIL;
  }
}

template<typename T>
ACE_INT32 DaoAssistant::Update(std::vector<T> &objs)
{
  try
  {
    odb::transaction t(GetDb()->begin());
    typename std::vector<T>::iterator iter = objs.begin();
    for(ACE_INT32 j = 0; iter != objs.end() ; ++iter, ++j)
    {
      GetDb()->update(*iter);
      if(j % 1000 == 0)
      {
        t.commit();
        t.reset(GetDb()->begin());
      }
    }
    t.commit();
    return UBP_SUCCESS;
  }
  catch(const odb::database_exception &e)
  {
    UBP_DAO_ERROR("DaoAssistant::Update vec failed: " << e.what());
    return UBP_FAIL;
  }
  catch(const odb::exception &e)
  {
    //std::cerr << e.what() << std::endl;
    UBP_DAO_ERROR("DaoAssistant::Update vec error: " << e.what());
    return UBP_FAIL;
  }
}

}//namespace dao
}//namespace platform
}//namespace ubp

#endif  //UBP_PLATFORM_MODULES_DAO_DAO_ASSISTANT_HPP_
