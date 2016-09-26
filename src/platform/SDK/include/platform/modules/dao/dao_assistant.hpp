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

/* 储存列字段名、列字段属性 */
/*typedef struct column_type_s
{
  std::string s_name;   //列字段名
  int i_value;          //字段属性，整形
  std::string s_value;  //字段属性，整形对应的字符串
} column_type;
*/
/* 储存列字段名、列字段属性vector，顺序按列从左到右 */
//typedef std::vector<column_type> vec_column_name;


/* 储存数据vector，元素为map，一个map为一行数据，map中元素为列名和值，值为string，需自己转换 */
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

    //重新设置事务的连接
    void Reset(DAOInfo& daoinfo);
    

    //提交
    bool Commit();
    
    //回滚
    bool Rollback();

    //是否已经提交或回滚过
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
    Returns:     0成功，非0失败
    Description: 连接数据库，设置连接池并发连接数最大值和最小值
    Parameter:   user用户名，passwd密码，database库名，host主机，port端口号
    Parameter:   max_connection连接池最大并发数，如果达到，则新的连接申请阻
                 塞并等待其他连接释放；如果设为0，则有连接时就创建新的
    Parameter:   min_connection连接池最小并发数，如果空闲连接超过最小值，则
                 关闭连接；如果设为0，则当连接释放时不关闭
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
    Returns:     >=0 成功，值为sql影响的结果条数；<0 失败
    Description: 执行原始SQL语句，使用ODB，主要包括增删改操作；查询操作不返回数据，无意义
    Parameter:   sql原始SQL语句
    ********************************************************************/
    virtual ACE_INT32 ExecuteNativeSql(const std::string &sql, DAOInfo *pDao=NULL) = 0;


    virtual ACE_INT32 ExecuteNativeSql(const std::string &sql, DBTransaction& trans) = 0;


    /********************************************************************
    Method:      ExecuteNativeSelCnt
    FullName:    DaoAssistant::ExecuteNativeSelCnt
    Access:      public
    Returns:     >=0 成功，值为select count的值；<0 失败
    Description: 执行原始SQL查询语句(select count)，直接使用MySQL接口，返回结果条数
    Parameter:   select原始SQL查询语句(select count)
    ********************************************************************/
    virtual ACE_INT32 ExecuteNativeSelCnt(const char *select, DAOInfo *pDao=NULL) = 0;


    virtual ACE_INT32 ExecuteNativeSelCnt(const char *select, DBTransaction& trans) = 0;


    /********************************************************************
    Method:      ExecuteNativeSelect
    FullName:    DaoAssistant::ExecuteNativeSelect
    Access:      public
    Returns:     0成功，1查询无结果，负值失败
    Description: 执行原始SQL查询语句，直接使用MySQL接口，返回查询结果和列字段名、类型
    Parameter:   records查询结果，vector中一个元素map为一行数据，map中元素为列名和值，值为string，需自己转换
                 sql原始SQL查询语句
    ********************************************************************/
    virtual ACE_INT32 ExecuteNativeSelect(vec_table_data &records,
                                          const char *select,
                                          DAOInfo *pDao=NULL) = 0;

    /********************************************************************
    Method:      ExecuteNativeSelect
    FullName:    DaoAssistant::ExecuteNativeSelect
    Access:      public
    Returns:     0成功，1查询无结果，负值失败
    Description: 执行原始SQL查询语句，直接使用MySQL接口，返回查询结果和列字段名、类型
    Parameter:   records查询结果，vector中一个元素为一行数据,第一行是列名,剩下的是每行的值
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
    Returns:     >=0 成功,影响结果条数；<0 失败
    Description: 批量SQL接口，执行原始SQL语句，直接使用MySQL接口，>0返回结果条数，<0错误码
    Parameter:   vector,原始SQL语句
    ********************************************************************/
    virtual ACE_INT32 ExecuteNativeBatchSql(const SQLStrVec &batch_sql,
        DAOInfo *pDao=NULL) = 0;


    /********************************************************************
    Method:      Find
    FullName:    DaoAssistant::Find
    Access:      public
    Returns:     0成功，非0失败
    Description: 查找对象表所有数据，无查询条件
    Parameter:   储存查询结果的vector
    ********************************************************************/
    template<typename T>
    ACE_INT32 Find(std::vector<T> &records);


    /********************************************************************
    Method:      Find
    FullName:    DaoAssistant::Find
    Access:      public
    Returns:     0成功，非0失败
    Description: 根据查询条件查找对象表的数据
    Parameter:   records储存查询结果的vector，query_cond对象型查询条件
    ********************************************************************/
    template<typename T>
    ACE_INT32 Find(std::vector<T> &records, odb::query<T> query_cond);


    /********************************************************************
    Method:      Find
    FullName:    DaoAssistant::Find
    Access:      public
    Returns:     0成功，非0失败
    Description: 根据查询条件查找对象表的数据
    Parameter:   records储存查询结果的vector，query_cond为SQL语句的查询条件
    ********************************************************************/
    template<typename T>
    ACE_INT32 Find(std::vector<T> &records, const std::string &query_cond);


    /********************************************************************
    Method:      Insert
    FullName:    DaoAssistant::Insert
    Access:      public
    Returns:     0成功，非0失败
    Description: 插入单个对象
    Parameter:   obj为单个对象
    ********************************************************************/
    template<typename T>
    ACE_INT32 Insert(T &obj);


    /********************************************************************
    Method:      Insert
    FullName:    DaoAssistant::Insert
    Access:      public
    Returns:     0成功，非0失败
    Description: 插入多个对象，每1000条提交一次
    Parameter:   obj为储存对象的vector
    ********************************************************************/
    template<typename T>
    ACE_INT32 Insert(std::vector<T> &objs);


    /********************************************************************
    Method:      Delete
    FullName:    DaoAssistant::Delete
    Access:      public
    Returns:     0成功，非0失败
    Description: 删除单个对象
    Parameter:   obj为单个对象
    ********************************************************************/
    template<typename T>
    ACE_INT32 Delete(T &obj);


    /********************************************************************
    Method:      Delete
    FullName:    DaoAssistant::Delete
    Access:      public
    Returns:     0成功，非0失败
    Description: 删除多个对象
    Parameter:   obj为储存多个对象的vector
    ********************************************************************/
    template<typename T>
    ACE_INT32 Delete(std::vector<T> &objs);


    /********************************************************************
    Method:      Update
    FullName:    DaoAssistant::Update
    Access:      public
    Returns:     0成功，非0失败
    Description: 更新单个对象,更新前先使用Find将对象查找出来
    Parameter:   obj为单个对象
    ********************************************************************/
    template<typename T>
    ACE_INT32 Update(T &obj);


    /********************************************************************
    Method:      Update
    FullName:    DaoAssistant::Update
    Access:      public
    Returns:     0成功，非0失败
    Description: 更新多个对象,更新前先使用Find将对象查找出来
    Parameter:   obj为储存多个对象的vector
    ********************************************************************/
    template<typename T>
    ACE_INT32 Update(std::vector<T> &objs);

  protected:

    Connection_type& getConnection(DBTransaction& trans);

  private:
    //获取当前数据库的指针
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
