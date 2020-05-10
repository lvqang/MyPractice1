#include<iostream>
#include<string>
#include<vector>
#include<memory>
#include<fstream>
#include<sstream>
#include<set>
#include<map>
#include<algorithm>//set_intersection head file
using namespace std;

//-------------------------------TextQuery-----------------------------------------
class QueryResult;
class TextQuery
{
public:
    using line_no = vector<string>::size_type;
    TextQuery(ifstream&);
    QueryResult query(const string&) const;
protected:

private:
    shared_ptr< vector<string> > file;//整个文本
    map< string, shared_ptr< set<line_no> > > wm;
};


//-------------------------------QueryResult-----------------------------------------
class QueryResult
{
    friend ostream& printqr(ostream&, const QueryResult&);
public:
    QueryResult(string s, shared_ptr< set<TextQuery::line_no> > p, shared_ptr< vector<string> > f):sought(s), lines(p), file(f) { }
    shared_ptr< vector<string> > get_file();//return shared_ptr<vector>
    const shared_ptr< set<TextQuery::line_no> > &get_lines()
    {
        return lines;
    }
protected:

private:
    string sought;
    shared_ptr< set<TextQuery::line_no> > lines;
    shared_ptr< vector<string> > file;//保存整个文本，根据lines提取所需的文本
};

<<<<<<< HEAD
ostream& printqr(ostream& os, const QueryResult& qr)
{
    os<<qr.sought<<" occurs "<<qr.lines->size()<<" "<<((qr.lines->size())>1 ? "times" : "time")<<endl;
    for( auto nu  : *(qr.lines) )
    {
        os<<"\t(line"<<nu+1<<")"<<(*(qr.file))[nu]<<endl;
    }
    return os;
=======
ostream& print(ostream& os, const QueryResult& qr)
{
        os<<qr.sought<<" occurs "<<qr.lines->size()<<" "<<(qr.lines->size()>1) ? "times" : "time"<<endl;
        for( auto nu  : *(qr.lines) )
        {
                os<<"\t(line"<<nu+1<<")"<<( *(qr.file) )[nu]<<endl;
        }
        return os;
>>>>>>> 042c0b011e3912226f1822a96560d80ad69fa97f
}


//-------------------------------Query_base-----------------------------------------
class Query_base//
{
    friend class Query;
public:

protected:
    using line_no = TextQuery::line_no;
    virtual ~Query_base() = default;
private:
<<<<<<< HEAD
    virtual QueryResult eval(const TextQuery&) const=0;//调用查询函数
    virtual string rep() const = 0;//返回查询关键词
=======
        shared_ptr< vector<string> > file;//整个文本
        map< string, shared_ptr< set<line_no> > > wm;
>>>>>>> 042c0b011e3912226f1822a96560d80ad69fa97f
};
//-------------------------------Query-----------------------------------------

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
    //ostream& operator<<(ostream &os, const Query &query)// not used
    //{
    //    return os << query.rep();
    //}
   // shared_ptr< vector<string> > get_file();
protected:

private:
    Query(shared_ptr<Query_base> query): q(query) {}
    shared_ptr<Query_base> q;
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
<<<<<<< HEAD
    {
        return t.query(query_word);
    }
    string rep() const
    {
        return query_word;
    }
    string query_word;
=======
	{
		return t.query(query_word);
        }
	string rep() const
        {
		return query_word;
        }
	string query_word;
>>>>>>> 042c0b011e3912226f1822a96560d80ad69fa97f
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
<<<<<<< HEAD
    BinaryQuery(const Query &l, const Query &r, string s): lhs(l), rhs(r), opSym(s) { }
    string rep() const
    {
        return "(" + lhs.rep() + " " + opSym + " " + rhs.rep() + ")";
    }
=======
	BinaryQuery(const Query &l, const Query &r, string s): lhs(l), rhs(r), opSym(s) { }
	string rep() const
	{
		return "(" + lhs.rep() + " " + opSym + " " + rhs.rep() + ")";
	}
>>>>>>> 042c0b011e3912226f1822a96560d80ad69fa97f
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




