#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include <stdlib.h>
using namespace std;
#include "header.h"

bool isSet_dataCalled=false;

class Json{
    public:
        class Key{
            public:
                string key;
                DataType valueType;
        };
        //########################################################################
        class KeyValue:public Key{
            public:
                string valueString;
                int valueInt;
                double valueDouble;
                string valueBool;
                //########################################################################
                void setKeyValue(string key,string value){
                    this->key=key;
                    this->valueType=STRING_TYPE;
                    this->valueString=value;
                }
                //########################################################################
                void setKeyValue(string key,int value){
                    this->key=key;
                    this->valueType=INT_TYPE;
                    this->valueInt=value;
                }
                //########################################################################
                void setKeyValue(string key,double value){
                    this->key=key;
                    this->valueType=DOUBLE_TYPE;
                    this->valueDouble=value;
                }
                //########################################################################
                void setKeyValue(string key,string value,bool v){
                    this->key=key;
                    this->valueType=BOOL_TYPE;
                    this->valueBool=value;
                }
                //########################################################################
                void decodingKeyValueString(string str){
                    int i;
                    string key;
                    DataType valueType=STRING_TYPE;
                    string value;

                    for(i=1;((str[i]!='"')&&(i<str.size()));i++){
                        key.push_back(str[i]);
                    }
                    for(i++;((str[i]!='"')&&(i<str.size()));i++);
                    for(i++;((str[i]!='"')&&(i<str.size()));i++){
                        value.push_back(str[i]);
                    }
                    setKeyValue(key,value);
                }
                //########################################################################
                void decodingKeyValueInt(string str){
                    int i;
                    string key;
                    DataType valueType=STRING_TYPE;
                    int value;
                    string tempString;

                    for(i=1;((str[i]!='"')&&(i<str.size()));i++){
                        key.push_back(str[i]);
                    }
                    for(;((!isdigit(str[i]))&&(i<str.size()));i++);
                    for(;((isdigit(str[i]))&&(i<str.size()));i++){
                        tempString.push_back(str[i]);
                    }
                    value=getDigit(tempString);
                    setKeyValue(key,value);
                }
                //########################################################################
                void decodingKeyValueDouble(string str){
                    int i;
                    string key;
                    DataType valueType=STRING_TYPE;
                    string tempString;
                    double value;

                    for(i=1;((str[i]!='"')&&(i<str.size()));i++){
                        key.push_back(str[i]);
                    }
                    for(;((!isDouble(str[i]))&&(i<str.size()));i++);
                    for(;((isDouble(str[i]))&&(i<str.size()));i++){
                        tempString.push_back(str[i]);
                    }
                    value=getDouble(tempString);
                    setKeyValue(key,value);
                }
                //########################################################################
                void decodingKeyValueBool(string str){
                    int i;
                    string key;
                    DataType valueType=STRING_TYPE;
                    string tempString;
                    string value;
                    bool v;

                    for(i=1;((str[i]!='"')&&(i<str.size()));i++){
                        key.push_back(str[i]);
                    }
                    for(;((!isBool(str[i]))&&(i<str.size()));i++);
                    if((str[i]=='t')||(str[i]=='T')){
                        value="true";
                    }else if((str[i]=='f')||(str[i]=='F')){
                        value="false";
                    }else if((str[i]=='n')||(str[i]=='N')){
						value="NULL";
					}else{
                        cerr<<">> unknown input <"<<str<<"> !!!"<<endl;
					}
                    setKeyValue(key,value,v);
                }
                //########################################################################
                void decodingKeyValue(string str){
                    int i;
                    for(i=0;((i<str.size())&&(str[i]!=':'));i++);
                    i++;
                    if(str[i]=='"'){
                        decodingKeyValueString(str);
                        return;
                    }else if(isBool(str[i])){
                        decodingKeyValueBool(str);
                        return;
                    }else if(isDouble(str[i])){
                        for(;i<str.size();i++){
                            if(str[i]=='.'){
                                decodingKeyValueDouble(str);
                                return;
                            }
                        }
                        decodingKeyValueInt(str);
                        return;
                    }

                }
                //########################################################################
                void decodingValue(string str){
					if(isDouble(str)){
						valueType=DOUBLE_TYPE;
						valueDouble=getDouble(str);
					}else if(isdigit(str)){
						valueType=INT_TYPE;
						valueInt=getDigit(str);
					}else if(isBool(str[0])){
						valueType=BOOL_TYPE;
						valueBool=getBool(str[0]);
					}else if(isString(str)){
						valueType=STRING_TYPE;
						valueString=str;
					}
				}
				//########################################################################
                void printValue(){
                    if(this->valueType==STRING_TYPE){
                        cout<<"> \""<<this->valueString<<"\""<<endl;
                    }else if(this->valueType==INT_TYPE){
                        cout<<"> "<<this->valueInt<<endl;
                    }else if(this->valueType==DOUBLE_TYPE){
                        cout<<"> "<<this->valueDouble<<endl;
                    }else if(this->valueType==BOOL_TYPE){
                        if(valueBool=="true"){
							cout<<"> "<<"True"<<endl;
						}else if(valueBool=="false"){
							cout<<"> "<<"False"<<endl;
						}else if(valueBool=="NULL"){
							cout<<"> "<<"Null"<<endl;
						}
                    }else{
                        cerr<<">> unknown type can't print it !!!"<<endl;
                    }
                }
        };
        //########################################################################
        class Container:public Key{
            public:
                vector<Container> containerVector;
                vector<KeyValue> keyValueVector;
                //########################################################################
                void setObject(string key,vector<KeyValue> inputKeyValueVector){
                    this->key=key;
                    for(int i=0;i<inputKeyValueVector.size();i++){
                        keyValueVector.push_back(inputKeyValueVector.at(i));
                    }
                }
                //########################################################################
                 vector<string> took(string str){
                    vector<string> result;
                    string temp;
                    int counter=0;
                    int innerScopeNumber=0;

                    for(int i=0;i<str.size();i++){
                        if(str[i]=='"'){
                            i++;
                            while(str[i]!='"')i++;
                        }
                        if(str[i]=='{'){
                            while(str[i]!='}')i++;
                        }
                        if(str[i]=='['){
                            while(str[i]!=']')i++;
                        }
                        if(str[i]==',')counter++;
                    }
                    counter++;

                    for(int i=0,j=0;i<counter;i++){
                        temp="";
                        for(;j<str.size();j++){
                            if(str[j]=='{')innerScopeNumber++;
                            if(str[j]=='[')innerScopeNumber++;
                            if(str[j]=='}')innerScopeNumber--;
                            if(str[j]==']')innerScopeNumber--;
                            if((innerScopeNumber==0)&&(str[j]==','))break;
                            temp.push_back(str[j]);
                        }
                        j++;
                        result.push_back(temp);
                    }
                    return result;
                }
                //########################################################################
                void decoding(string str){
                    vector<string> tookedStr;
                    tookedStr=took(str);

                    for(int i=0;i<tookedStr.size();i++){
                        if(!isObjectOrArray(tookedStr.at(i))){
                            KeyValue tempKeyValue;
                            tempKeyValue.decodingKeyValue(tookedStr.at(i));
                            keyValueVector.push_back(tempKeyValue);
                        }else if(isObject(tookedStr.at(i))){
                            Container container;
                            container.key=getObjectAndArrayKey(tookedStr.at(i));
                            container.valueType=OBJECT_TYPE;
                            container.decoding
                                (eraseObjectOrArrayKeyAndFirstAndLastIndex
                                 (tookedStr.at(i))
                                );
                            containerVector.push_back(container);
                        }else {
                            Container container;
                            container.key=getObjectAndArrayKey(tookedStr.at(i));
                            container.valueType=ARRAY_TYPE;
                            container.decodingArray
                                (eraseObjectOrArrayKeyAndFirstAndLastIndex
                                 (tookedStr.at(i))
                                );
                            containerVector.push_back(container);
                        }
                    }
                }
                //########################################################################
                void decodingArray(string str){
                    vector<string> tookedStr;
                    tookedStr=took(str);
                    for(int i=0;i<tookedStr.size();i++){
                        if(isObject(tookedStr.at(i))){
                            Container container;
                            container.key=intToString(i);
                            container.valueType=OBJECT_TYPE;
                            container.decoding
                                (eraseObjectOrArrayKeyAndFirstAndLastIndex
                                 (tookedStr.at(i))
                                );
                            containerVector.push_back(container);
                        }else if(isArray(tookedStr.at(i))){
							Container container;
                            container.key=intToString(i);
                            container.valueType=ARRAY_TYPE;
                            container.decodingArray
                                (eraseObjectOrArrayKeyAndFirstAndLastIndex
                                 (tookedStr.at(i))
                                );
                            containerVector.push_back(container);
						}else{
							KeyValue keyValue;
							keyValue.key=intToString(i);
							keyValue.decodingValue(tookedStr.at(i));
							keyValueVector.push_back(keyValue);
						}
                    }

                }

        };
        //########################################################################
        static string deleteSpaceAndEnter(string str){
            string result;
            bool isInsideString=false;
            for(int i=0;i<str.size();i++){
                if(str[i]=='"'){
					isInsideString=!isInsideString;
				}
				if(((str[i]==' ')||(str[i]=='\n'))&&(!isInsideString)){
					continue;
				}
				result.push_back(str[i]);
            }
            return result;
        }
        //########################################################################
        static void start(){
            Container container;
            while(true){
                getInputCommand(container);
                cout<<endl<<"__________________________________________________________"<<endl;
            }
        }
        //########################################################################
        static void getInputCommand(Container& container){
            string inputCommand;
            char temp[5001];
            cout<<"\nEnter command"<<endl<<"-> ";
            cin>>inputCommand;
            if(inputCommand=="set-data"){
                set_data(container);
                isSet_dataCalled=true;
            }else if(inputCommand=="get-value"){
                if(isSet_dataCalled){
                    get_value(container);
                }else{
                    cin.getline(temp,5000);
                    cerr<<">> you can't use <get-value> command because there is no data !!!"<<endl;
                }
            }else if(inputCommand=="get-type"){
                if(isSet_dataCalled){
                    get_type(container);
                }else{
                    cin.getline(temp,5000);
                    cerr<<">> you can't use <get-type> command because there is no data !!!"<<endl;
                }
            }else if(inputCommand=="compare"){
                if(isSet_dataCalled){
                    compare(container);
                }else{
                    cin.getline(temp,5000);
                    cerr<<">> you can't use <compare> command because there is no data !!!"<<endl;
                }
            }else if(inputCommand=="calculate"){
                if(isSet_dataCalled){
                    calculate(container);
                }else{
                    cin.getline(temp,5000);
                    cerr<<">> you can't use <calculate> command because there is no data !!!"<<endl;
                }
            }else if(inputCommand=="exit"){
                exit(0);
            }else if(inputCommand=="help"){
				cout<<"\n valid inputs :\n"<<endl;
				cout<<"\t(set-data): set-data < Json data >\n"<<endl;
				cout<<"\t(get-value): get-value < Path >\n"<<endl;
				cout<<"\t(get-type): get-type < Path >\n"<<endl;
				cout<<"\t(compare): compare < Path(1) > < Path(2) >\n"<<endl;
				cout<<"\t(calculate): calculate < Path(1) > < operator > < Path(2) >\n"<<endl;
            }else{
				cerr<<">> invalid command !!! (use help command for more information)"<<endl;
            }
        }
		//########################################################################
        static void set_data(Container& container){
            if((container.containerVector.size()>0)||(container.keyValueVector.size()>0)){
                cout<<"> old data is deleted !"<<endl;
                container.containerVector.clear();
                container.keyValueVector.clear();
            }

            string str=getInputDataFromUser();
            str=deleteSpaceAndEnter(str);

            if(isValidInput(str)){
                cout<<"> new data added !"<<endl;
                container.decoding
                    (eraseObjectOrArrayKeyAndFirstAndLastIndex(str));
            }
        }
        //########################################################################
        static void get_value(Container container){
            string inputPath;
            cin>>inputPath;
            vector<string> tookedInputPath=cutAccordingToDote(inputPath);

            Container resultContainer=
                pathContainer(container,1,tookedInputPath);

            KeyValue resultKeyValue=
                pathKeyValue(container,1,tookedInputPath);

            if((resultContainer.key==".")&&(resultKeyValue.key==".")){
                cerr<<">> <"<<inputPath<<">  path not exist "<<endl;
            }else{
                if(resultContainer.key!="."){
                    cout<<"> "<<getTypeName(resultContainer.valueType)<<endl;
                }else{
                    resultKeyValue.printValue();
                }
            }
        }
		//########################################################################
        static void get_type(Container container){
            string inputPath;
            cin>>inputPath;
            vector<string> tookedInputPath=cutAccordingToDote(inputPath);

            Container resultContainer=
                pathContainer(container,1,tookedInputPath);

            KeyValue resultKeyValue=
                pathKeyValue(container,1,tookedInputPath);

            if((resultContainer.key==".")&&(resultKeyValue.key==".")){
                cerr<<">> <"<<inputPath<<">  path not exist "<<endl;
            }else{
                if(resultContainer.key!="."){
                    cout<<"> "<<getTypeName(resultContainer.valueType)<<endl;
                }else{
                    cout<<"> "<<getTypeName(resultKeyValue.valueType)<<endl;
                }
            }
        }
        //########################################################################
        static void compare(Container container){
            string inputPath1;
            cin>>inputPath1;
            vector<string> tookedInputPath1=cutAccordingToDote(inputPath1);

            string inputPath2;
            cin>>inputPath2;
            vector<string> tookedInputPath2=cutAccordingToDote(inputPath2);

            KeyValue resultKeyValue1=
                pathKeyValue(container,1,tookedInputPath1);

			KeyValue resultKeyValue2=
                pathKeyValue(container,1,tookedInputPath2);

            if(resultKeyValue1.key=="."){
            	cerr<<">> <"<<inputPath1<<">  path not exist "<<endl;
            	return;
			}
			if(resultKeyValue2.key=="."){
            	cerr<<">> <"<<inputPath2<<">  path not exist "<<endl;
            	return;
			}

			if(resultKeyValue1.valueType!=resultKeyValue2.valueType){
				if(!((resultKeyValue1.valueType==INT_TYPE&&resultKeyValue2.valueType==DOUBLE_TYPE)||
				   (resultKeyValue2.valueType==INT_TYPE&&resultKeyValue1.valueType==DOUBLE_TYPE))){
				   		cerr<<">> <different data Types> can't compare !!!"<<endl;
				   		return;
				   }
			}


			if((resultKeyValue1.valueType==STRING_TYPE)&&(resultKeyValue2.valueType==STRING_TYPE)){
				if(resultKeyValue1.valueString==resultKeyValue2.valueString){
					cout<<"> Equals"<<endl;
				}if(resultKeyValue1.valueString>resultKeyValue2.valueString){
					cout<<"> Left"<<endl;
				}if(resultKeyValue1.valueString<resultKeyValue2.valueString){
					cout<<"> Right"<<endl;
				}
				return;
			}

        	double resultDouble1;
        	double resultDouble2;
            if(resultKeyValue1.valueType==INT_TYPE){
            	resultDouble1=(int)resultKeyValue1.valueInt;
			}
            if(resultKeyValue2.valueType==INT_TYPE){
            	resultDouble2=(int)resultKeyValue2.valueInt;
			}
			if(resultKeyValue1.valueType==DOUBLE_TYPE){
            	resultDouble1=resultKeyValue1.valueDouble;
			}
            if(resultKeyValue2.valueType==DOUBLE_TYPE){
            	resultDouble2=resultKeyValue2.valueDouble;
			}
			if(resultDouble1==resultDouble2){
				cout<<"> Equals"<<endl;
			}if(resultDouble1>resultDouble2){
				cout<<"> Left"<<endl;
			}if(resultDouble1<resultDouble2){
				cout<<"> Right"<<endl;
			}
        }
        //########################################################################
        static void calculate(Container container){
            string inputPath1;
            cin>>inputPath1;
            vector<string> tookedInputPath1=cutAccordingToDote(inputPath1);

            char inputOperatorChar;
            cin>>inputOperatorChar;

            string inputPath2;
            cin>>inputPath2;
            vector<string> tookedInputPath2=cutAccordingToDote(inputPath2);

            KeyValue resultKeyValue1=
                pathKeyValue(container,1,tookedInputPath1);

			KeyValue resultKeyValue2=
                pathKeyValue(container,1,tookedInputPath2);

            if(resultKeyValue1.key=="."){
            	cerr<<">> <"<<inputPath1<<">  path not exist "<<endl;
            	return;
			}
			if(resultKeyValue2.key=="."){
            	cerr<<">> <"<<inputPath2<<">  path not exist "<<endl;
            	return;
			}

			if((resultKeyValue1.valueType==STRING_TYPE)||(resultKeyValue2.valueType==STRING_TYPE)){
				cerr<<">> can't calculate in string type !!!'"<<endl;
            	return;
			}

			if((resultKeyValue1.valueType==BOOL_TYPE)||(resultKeyValue2.valueType==BOOL_TYPE)){
				cerr<<">> can't calculate in boolean type !!!'"<<endl;
            	return;
			}

			if(!((inputOperatorChar=='+')||
			  (inputOperatorChar=='-')||
			  (inputOperatorChar=='*')||
			  (inputOperatorChar=='/')||
			  (inputOperatorChar=='^'))){
				cerr<<">> inputed operator is invalid !!!(valid operators : + - * / ^ )"<<endl;
            	return;
			  }

			double resultDouble1;
        	double resultDouble2;
        	double answerDouble;
            if(resultKeyValue1.valueType==INT_TYPE){
            	resultDouble1=(int)resultKeyValue1.valueInt;
			}
            if(resultKeyValue2.valueType==INT_TYPE){
            	resultDouble2=(int)resultKeyValue2.valueInt;
			}
			if(resultKeyValue1.valueType==DOUBLE_TYPE){
            	resultDouble1=resultKeyValue1.valueDouble;
			}
            if(resultKeyValue2.valueType==DOUBLE_TYPE){
            	resultDouble2=resultKeyValue2.valueDouble;
			}

			switch(inputOperatorChar){
				case '+':{
					answerDouble=resultDouble1+resultDouble2;
					break;
				}
				case '-':{
					answerDouble=resultDouble1-resultDouble2;
					break;
				}
				case '*':{
					answerDouble=resultDouble1*resultDouble2;
					break;
				}
				case '/':{
					answerDouble=resultDouble1/resultDouble2;
					break;
				}
				case '^':{
					answerDouble=pow(resultDouble1,resultDouble2);
					break;
				}
			}
			cout<<"> "<<answerDouble<<endl;

        }
        //########################################################################
        static Container pathContainer(Container container,int n,vector<string> inputPath){
            if(inputPath.size()==n)return container;

            for(int i=0;i<container.containerVector.size();i++){
                if(container.containerVector.at(i).key==inputPath.at(n)){
                    return pathContainer(container.containerVector.at(i),n+1,inputPath);
                }
            }
            Container emptyContainer;
            emptyContainer.key=".";
            return emptyContainer;
        }
        //########################################################################
        static KeyValue pathKeyValue(Container container,int n,vector<string> inputPath){
            if((inputPath.size()==n+1)){
                string lastPath=inputPath.at(inputPath.size()-1);

                for(int i=0;i<container.keyValueVector.size();i++){
                    if(container.keyValueVector.at(i).key==lastPath){
                        return container.keyValueVector.at(i);
                    }
                }

                KeyValue emptyKeyValue;
                emptyKeyValue.key=".";
                return emptyKeyValue;
            }else{
                for(int i=0;i<container.containerVector.size();i++){
                    if(container.containerVector.at(i).key==inputPath.at(n)){
                        return pathKeyValue(container.containerVector.at(i),n+1,inputPath);
                    }
                }
                KeyValue emptyKeyValue;
                emptyKeyValue.key=".";
                return emptyKeyValue;
			}
		}
        //########################################################################
        static bool isValidInput(string str){
            int countOfStartObject=countOfRepetiion(str,'{');
            int countOfEndObject=countOfRepetiion(str,'}');
            if(countOfEndObject!=countOfStartObject){
                cerr<<">> input json have error !!!"<<endl;
                if(countOfEndObject>countOfStartObject){
                    cerr<<">> need "<<countOfEndObject-countOfStartObject<<" Object start < { > ."<<endl;
                }else{
                    cerr<<">> need "<<countOfStartObject-countOfEndObject<<" Object end < } > ."<<endl;
                }
                return false;
            }

            int countOfStartArray=countOfRepetiion(str,'[');
            int countOfEndArray=countOfRepetiion(str,']');
            if(countOfEndArray!=countOfStartArray){
                cerr<<">> input json have error !!!"<<endl;
                if(countOfEndArray>countOfStartArray){
                    cerr<<">> need ("<<countOfEndArray-countOfStartArray<<") Array start < [ > ."<<endl;
                }else{
                    cerr<<">> need ("<<countOfStartArray-countOfEndArray<<") Array end < ] > ."<<endl;
                }
                return false;
            }

            int countOfDoubleCote=countOfRepetiion(str,'"');
            if(countOfDoubleCote%2==1){
                cerr<<">> input json have error !!!"<<endl;
                cerr<<">> number of < \" > is odd !!! (have to be even)"<<endl;
                return false;
            }
            return true;
        }
        //########################################################################
};

int main(){
	Json::start();
    return 0;
}
