#ifndef UBP_PLATFORM_MODULES_DAO_DAO_NATIVE_HPP_
#define UBP_PLATFORM_MODULES_DAO_DAO_NATIVE_HPP_


#include "dao_assistant.hpp"

namespace ubp
{
namespace platform
{
namespace dao
{

class DAO_IMPORT_EXPORT Dao_NativeOperators
{
public:
	
	    virtual ~Dao_NativeOperators(){}

    /********************************************************************
       Method:      ExecuteNativeSql
       FullName:    DaoAssistant::ExecuteNativeSql
       Access:      public
       Returns:     >=0 成功，值为sql影响的结果条数；<0 失败
       Description: 执行原始SQL语句，使用ODB，主要包括增删改操作；查询操作不返回数据，无意义
       Parameter:   sql原始SQL语句
       ********************************************************************/
       virtual ACE_INT32 ExecuteNativeSql(const char* sql, Connection_type& conn) = 0;
    
    
       /********************************************************************
       Method:      ExecuteNativeSelCnt
       FullName:    DaoAssistant::ExecuteNativeSelCnt
       Access:      public
       Returns:     >=0 成功，值为select count的值；<0 失败
       Description: 执行原始SQL查询语句(select count)，直接使用MySQL接口，返回结果条数
       Parameter:   select原始SQL查询语句(select count)
       ********************************************************************/
       virtual ACE_INT32 ExecuteNativeSelCnt(const char *select, Connection_type& conn) = 0;
    
    
       /********************************************************************
       Method:      ExecuteNativeSelect
       FullName:    DaoAssistant::ExecuteNativeSelect
       Access:      public
       Returns:     0成功，1查询无结果，负值失败
       Description: 执行原始SQL查询语句，直接使用MySQL接口，返回查询结果和列字段名、类型
       Parameter:   records查询结果，vector中一个元素map为一行数据，map中元素为列名和值，值为string，需自己转换
                    sql原始SQL查询语句
       ********************************************************************/
       virtual ACE_INT32 ExecuteNativeSelect(vec_table_data& records,
                                             const char *select,
                                             Connection_type& conn) = 0;
   
      /********************************************************************
      Method:      ExecuteNativeSelect
      FullName:    DaoAssistant::ExecuteNativeSelect
      Access:      public
      Returns:     0成功，1查询无结果，负值失败
      Description: 执行原始SQL查询语句，直接使用MySQL接口，返回查询结果和列字段名、类型
      Parameter:   records查询结果，vector中一个元素为一行数据,第一行是列名,剩下的是每行的值
      ********************************************************************/
      virtual ACE_INT32 ExecuteNativeSelect(vec_raw_table_data &records, const char *select, Connection_type& conn)=0;
    
       /********************************************************************
       Method:      ExecuteNativeBatchSql
       FullName:    DaoAssistant::ExecuteNativeBatchSql
       Access:      public
       Returns:     >=0 成功,影响结果条数；<0 失败
       Description: 批量SQL接口，执行原始SQL语句，直接使用MySQL接口，>0返回结果条数，<0错误码
       Parameter:   vector,原始SQL语句
       ********************************************************************/
       virtual ACE_INT32 ExecuteNativeBatchSql(const SQLStrVec &batch_sql,
           Connection_type& conn) = 0;
        
        
  };
}
}
}
#endif 


