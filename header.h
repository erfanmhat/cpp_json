#ifndef HEAADER_H
#define HEAADER_H

typedef char DataType;

const DataType STRING_TYPE = 1;
const DataType INT_TYPE = 2;
const DataType DOUBLE_TYPE = 3;
const DataType BOOL_TYPE = 4;
const DataType OBJECT_TYPE = 5;
const DataType ARRAY_TYPE = 6;

//########################################################################
int countOfRepetiion(string str,char ch){
    int result=0;
    for(int i=0;i<str.size();i++){
            if(str[i]==ch)result++;
    }
    return result;
}
//########################################################################
string getInputDataFromUser(){
	string result;
	char tempString[5001];
	int scopeStep=0;
	while(true){
		cin.getline(tempString,5000);
		for(int i=0;tempString[i]!='\0';i++){
			if((tempString[i]=='{')||(tempString[i]=='['))scopeStep++;
			if((tempString[i]=='}')||(tempString[i]==']'))scopeStep--;
		}
		for(int i=0;tempString[i]!='\0';i++){
			result.push_back(tempString[i]);
		}
		if(scopeStep==0)break;
	}
	return result;
}
//########################################################################
string intToString(int inputInt){
    if(inputInt==0)return "0";
    string result;
    char temp;
    if(inputInt<0){
        inputInt=(-inputInt);
        result.push_back('-');
    }
    for(int i=0;inputInt>0;i++){
        result.push_back((char)((inputInt%10)+48));
        inputInt/=10;
    }
    for(int i=0;i<result.size()/2;i++){
        temp=result[i];
        result[i]=result[result.size()-1-i];
        result[result.size()-1-i]=temp;
    }
    return result;
}
//########################################################################
vector<string> cutAccordingToDote(string str){
    vector<string> result;
    string tempString;

    result.push_back("");
    for(int i=0;i<str.size();i++){
        if(str[i]!='.'){
            tempString.push_back(str[i]);
        }else{
            result.push_back(tempString);
            tempString="";
        }
    }
    result.push_back(tempString);
    return result;
}
//########################################################################
string getTypeName(DataType dataType){
    if(dataType==STRING_TYPE)return "String";
    else if(dataType==INT_TYPE)return "Number";
    else if(dataType==DOUBLE_TYPE)return "Number";
    else if(dataType==BOOL_TYPE)return "Bool";
    else if(dataType==OBJECT_TYPE)return "Object";
    else if(dataType==ARRAY_TYPE)return "Array";
    else return "unknown!";
}
//########################################################################
DataType getObjectOrArrayType(string str){
    int i;

    for(i=0;i<str.size();i++){
        if((str[i]=='{')||(str[i]=='['))break;
    }
    if(str[i]=='{'){
        return OBJECT_TYPE;
    }else if(str[i]=='['){
        return ARRAY_TYPE;
    }else{
        return -1;
    }
}
//########################################################################
string getObjectAndArrayKey(string str){
    if((str[0]=='{')||(str[0]=='[')){
        return "";
    }else{
        string tempString;
        for(int i=1;((i<str.size())&&(str[i]!='"'));i++){
            tempString.push_back(str[i]);
        }
        return tempString;
    }
}
//########################################################################
string eraseObjectOrArrayKeyAndFirstAndLastIndex(string str){
    string result;
    int i;

    for(i=0;i<str.size();i++){
        if((str[i]=='{')||(str[i]=='['))break;
    }

    for(i++;i<str.size()-1;i++){
            result.push_back(str[i]);
    }
    return result;

}
//########################################################################
bool isObject(string str){
    for(int i=0;i<str.size();i++){
        if((str[i]=='[')||(str[i]==']'))return false;
        if((str[i]=='{')||(str[i]=='}')){
             return true;
        }
    }
    return false;
}
//########################################################################
bool isArray(string str){
    for(int i=0;i<str.size();i++){
        if((str[i]=='{')||(str[i]=='}'))return false;
        if((str[i]=='[')||(str[i]==']')){
             return true;
        }
    }
    return false;
}
//########################################################################
bool isObjectOrArray(string str){
    for(int i=0;i<str.size();i++){
        if((str[i]=='{')||(str[i]=='}')||(str[i]=='[')||(str[i]==']')){
             return true;
        }
    }
    return false;
}
//########################################################################
int getDigit(string str){
    int result=0;
    for(int i=0;i<str.size();i++){
        result*=10;
        result+=str[i]-48;
    }
    return result;
}
//########################################################################
string getBool(char ch){
    if((ch=='f')||(ch=='F')){
        return "false";
    }else if((ch=='t')||(ch=='T')){
        return "true";
    }else if((ch=='n')||(ch=='N')){
        return "NULL";
    }
    return "false";
}
//########################################################################
bool isString(string str){
    bool result=false;
    if((str[0]=='"')&&(str[str.size()-1]=='"'))result=true;
    return result;
}
//########################################################################
bool isDouble(char ch){
    if((isdigit(ch))||(ch=='.')){
        return true;
    }else{
        return false;
    }
}
//########################################################################
bool isdigit(string str){
    bool result=true;
    for(int i=0;i<str.size();i++){
        if(!isdigit(str[i]))result=false;
    }
    return result;
}
//########################################################################
bool isDouble(string str){
    bool result=true;
    for(int i=0;i<str.size();i++){
        if(!isDouble(str[i]))result=false;
    }
    return result;
}
//########################################################################
bool isBool(char ch){
    if((ch=='t')||(ch=='f')||(ch=='T')||(ch=='F')||(ch=='N')||(ch=='n')){
        return true;
    }else{
        return false;
    }
}
//########################################################################
double getDouble(string str){
    double result=0;
    int i;
    for(i=0;((i<str.size())&&(str[i]!='.'));i++){
        result*=10;
        result+=str[i]-48;
    }
    i++;
    for(double j=10;i<str.size();i++,j*=10){
        result+=(str[i]-48)/j;
    }
    return result;
}
//########################################################################
#endif //HEADER_H
