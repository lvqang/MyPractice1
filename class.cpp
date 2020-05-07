#include"class.h"

//---------------------QueryResult-------------------
    hared_ptr< vector<string> > QueryResult::get_file()
    {
        return file;
    }
//-------------------QueryResult end-----------------

//--------------------------------------TextQuery---------------------
        TextQuery:: TextQuery(ifstream &is): file(new vector<string>)
        {
                string text;
                while( getline(is, text) )//按行读入文本
                {
                        file->push_back(text);
                        int no = file->size()-1;//行号
                        istringstream line(text);//字符串流绑定每一行
                        string word;
                        while( line>>word )
                        {
                                auto &lines = wm[word];
                                if( ! lines )
                                {
                                        lines.reset( new set<line_no> );//此处尝试不用reset函数
                                }
                                lines->insert(no);
                        }
                }
        }


QueryResult TextQuery::  query(const string &str) const
{
       // shared_ptr<set<line_no>> lines = wm[str];//使用下标运算会导致map创建新的成员
        shared_ptr< vector<string> > files = file;
        if(  wm.find(str) == wm.end() )
        {
                static shared_ptr< set<line_no> > liness = new set<line_no>;//静态变量，生存周期最长，防止指针无效
                return QueryResult(str, liness, files);
        }
        else  return QueryResult(str, wm[str], files);
}
//----------------------------------TextQuery end--------------




//--------------------------------------Query_base--------------------
QueryResult OrQuery::eval(const TextQuery &text) const
{
        auto right = rhs.eval(text);
        auto left   = lhs.eval(text);

        auto ret_lines = make_shared< set<line_no> >( left.begin(), left.end() );

        ret_lines->insert( right.begin(), right.end() );

        return QueryResult( rep(), ret_lines, left.get_file() );
}


QueryResult AndQuery::eval(const TextQuery &text) const
{
        auto right = rhs.eval(text);
        auto left   = lhs.eval(text);

        auto ret_lines = make_shared< set<line_no> >( );
        set_intersection( left.begin(), left.end(), right.begin(), right.end(), inserter( *ret_lines, ret_lines->begin() ) );

        return QueryResult( rep(), ret_lines, left.get_file() );
}

QueryResult NotQuery::eval(const TextQuery &text) const
{
    auto result = query.eval(text);//WordQuery's eval

    auto ret_lines = make_shared< set<line_no> >( );
    auto left = make_shared< vector<string> >( );//creat string

    auto beg = result.lines->begin(), end = result.lines->end();//取其中set尝试

    auto sz = result.get_file()->size();//get vector<string>'s size计算得到所有行的个数？

    for(size_t n = 0; n!=sz; ++n)//改变方法
    {
        if(beg == end || *beg != n)
        {
            ret_lines->insert(n);
            left->push_back(*(text.file)[n]);
        }
        else if(beg != end)
            ++beg;
    }
    return QueryResult( rep(), ret_lines, left );
}



















