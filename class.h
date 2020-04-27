#include<iostream>
#include<string>
#include<vector>
#include<memory>
//#include<>
using namespace std;
class TextQuery
{
public:

protected:

private:

};

class QueryResult
{
public:

protected:

private:

};

class Query
{
    friend Query operator~(const Query&);
    friend Query operator~(const Query&, const Query&);
    friend Query operator~(const Query&, const Query&);
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
protected:

private:
    Query(shared_ptr<Query_base> query): q(query) {}
    shared_ptr<Query_base> q;
};


class Query_base//
{
    friend class Query;
public:

protected:

private:
    virtual QueryResult eval(const TextQuery&) const=0;
    virtual string rep() const = 0;
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
inline Query operator~(const Query &operand)
{
    return shared_ptr<Query_base> (new NotQuery(operand));
}

//===========================================
//Function:与或的基类
//
//===========================================
class BinaryQuery: public Query_base
{
public:

protected:
        BinaryQuery(const Query &l, const Query &r, const Query &r, string s): lhs(l), rhs(r), opSym(s) { }
        string rep() const
        {
                return "(" + lhs.rep() + " " + opSym + " " + rhs.rep() + ")";
        }
        Query lhs, rhs;
        string opSym;
private:

};

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




