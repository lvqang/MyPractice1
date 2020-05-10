#include"class.h"

int main(int argc, char *argv[])
{
    string file;//the detect file
    if(argc<2)
    {
        cout<<"Please enter file: ";
        cin>>file;
    }
    else
    {
        file = string(argv[1]);
    }
    ifstream is(file);//input file name
    TextQuery text(is);//query object

    while(1)
    {
        cout<<"Please enter string what you want query: ";
        string str;//query string
        getline(cin, str);//get query string
        string str1[3];//storage query string
        istringstream ist(str);
        for(size_t i=0; i<3; ++i)
        {
            ist>>str1[i];
        }
        if(str1[1].empty())
        {
            if( *(str1[0].begin()) =='~' )
            {
                str1[0].erase( str1[0].begin() );//delete first word ~
                Query que(str1[0]);
                Query notque = ~que;//not find;
                QueryResult qr = notque.eval(text);

                printqr(cout, qr);
                cout<<endl;
            }
            else
            {
                Query que(str1[0]);//word fine
                QueryResult qr = que.eval(text);

                printqr(cout, qr);
                cout<<endl;
            }
        }
        else
        {
            if(str1[1] == "&")
            {
                Query que1(str1[0]),que2(str1[2]);//and
                Query andque = que1&que2;
                QueryResult qr = andque.eval(text);

                printqr(cout, qr);
                cout<<endl;
            }
            else if(str1[1] == "|")
            {
                Query que1(str1[0]),que2(str1[2]);//or
                Query orque = que1|que2;
                QueryResult qr = orque.eval(text);

                printqr(cout, qr);
                cout<<endl;
            }
        }
    }
    return 0;
}

