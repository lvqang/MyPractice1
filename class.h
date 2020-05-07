#include<iostream>
#include<string>
#include<vector>
#include<memory>
#include<fstream>
#include<sstream>
#include<set>
#include<map>
using namespace std;

//-------------------------------QueryResult-----------------------------------------
class QueryResult
{
    friend ostream& print(ostream&, const QueryResult&);
public:
    QueryResult(string s, shared_ptr< set<TextQuery::line_no> > p, shared_ptr< vector<string> > f):sought(s), lines(p), file(f) { }
    shared_ptr< vector<string> > get_file();//return shared_ptr<vector>
protected:

private:
    string sought;
    shared_ptr< set<TextQuery::line_no> > lines;
    shared_ptr< vector<string> > file;
};


//-------------------------------TextQuery-----------------------------------------
class TextQuery
{
public:
        using line_no = vector<string>::size_type;
        TextQuery(ifstream&);
        QueryResult query(const string&) const;
protected:

private:
        shared_ptr< vector<string> > file;
        map< string, shared_ptr< set<line_no> > > wm;
};


//-------------------------------Query-----------------------------------------
class Query_base;
class Query                               //作为接口类
{
        friend Query operator~(const Query&);
        friend Query operator&(const Query&, const Query&);
        friend Query operator|(const Query&, const Query&);
public:
        Query(const string&);
        QueryResult eval(const TextQuery&t) const
        {
                return q->eval(t);
        }
        string rep() const
        {
                return q->rep();
        }
        ostream& operator<<(ostream &os, const Query &query)
        {
                return os << query.rep();
        }
   // shared_ptr< vector<string> > get_file();
protected:

private:
        Query(shared_ptr<Query_base> query): q(query) {}
        shared_ptr<Query_base> q;
};

//-------------------------------Query_base-----------------------------------------
class Query_base//
{
        friend class Query;
public:

protected:
        using line_no = TextQuery::line_no;
        virtual ~Query_base() = default;
private:
        virtual QueryResult eval(const TextQuery&) const=0;//调用查询函数
        virtual string rep() const = 0;//返回查询关键词
};



//===================================================
//Function:WordQuery class from Query_base
//
//===================================================
class WordQuery:public Query_base
{
        friend class Query;
public:

protected:

private:
        WordQuery(const string &s):query_word(s) {}//creat function
        QueryResult eval(const TextQuery &t) const
        {
                return t.query(query_word);
        }
        string rep() const
        {
                return query_word;
        }
        string query_word;
};

inline Query::Query(const string &s):q(new WordQuery(s)){}

//===========================================
//Function:NotQuery class from Query_base
//
//===========================================
class NotQuery:public Query_base
{
    friend Query operator~(const Query &);
public:

protected:

private:
    NotQuery(const Query &q):query(q) {}//creat function
    QueryResult eval(const TextQuery &) const;
    string rep() const
    {
        return "~(" + query.rep() + ")";
    }
    Query query;
};
inline Query operator~(const Query &operand)//Query的友元，可以访问私有构造函数
{
    return shared_ptr<Query_base> (new NotQuery(operand));//通过指针隐式转换为Query类
}

//===========================================
//Function:与或的基类
//
//===========================================
class BinaryQuery: public Query_base
{
public:

protected:
        BinaryQuery(const Query &l, const Query &r, string s): lhs(l), rhs(r), opSym(s) { }
        string rep() const
        {
                return "(" + lhs.rep() + " " + opSym + " " + rhs.rep() + ")";
        }
        Query lhs, rhs;
        string opSym;
private:

};

//-------------------------------AndQuery-----------------------------------------
class AndQuery: public BinaryQuery
{
        friend Query operator &(const Query&, const Query&);
public:

protected:

private:
        AndQuery(const Query &left, const Query &right): BinaryQuery(left, right, "&"){ }
        QueryResult eval(const TextQuery&)const;
};
inline Query operator &(const Query &lhs, const Query &rhs)
{
        return  shared_ptr<Query_base>( new AndQuery(lhs, rhs) );
}

//-------------------------------OrQuery-----------------------------------------
class OrQuery: public BinaryQuery
{
        friend Query operator|(const Query &, const Query &);
public:

protected:

private:
        OrQuery(const Query &left, const Query &right): BinaryQuery(left, right, "|"){ }
        QueryResult eval(const TextQuery &) const;
};
inline Query operator |(const Query &lhs, const Query &rhs)
{
        return  shared_ptr<Query_base>( new OrQuery(lhs, rhs) );
}




