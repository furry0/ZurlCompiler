#include<iostream>
#include<cstdio>
#include<string>
#include<map>
#include<stack>
using namespace std;
bool isNum(char c){
	return c >= '0'&&c <= '9';
}
bool isLetter(char c){
	return c >= 'A'&&c <= 'z';
}
bool isBac(char c){
	return c == '{' || c == '}' || c == '(' || c == ')';
}
#define IS_NUM 1
#define IS_LET 2
#define IS_OPE 3
#define NO word_ori[render_now]
#define NT word_type[render_now]
string word_ori[100];
string word_type[100];
int word_now;
int global_var_num[100];
string global_var_str[100];
int render_now;
class var{
	int type;
	int site;
};
class sys_lib_class{
public:
	int stoi(string x){
		int ret =0;
		for (int i = 0; i <= x.length() - 1; i++){
			ret = ret * 10 + x[i] - '0';
		}
		return ret;
	}
	string itos(int x){
		string ret;
		if (x == 0)return "0";
		while (x){
			ret = char(x % 10 + '0') + ret;
			x /= 10;
		}
		return ret;
	}
}sys_lib;
class excute_controller{
public:
	map<string, var> global_var;
	bool initrender(){
		render_now = 0;
	}
	bool isVar(){
		return NT == "letter" && global_var.find(NO) != global_var.end();
	}
	bool calExp(){
		//int version
		stack<int> ope;
		stack<int> num;
		if (NT == "num")num.push(sys_lib.stoi(NO));
		if (NT == "str")cout << "ERR" << endl;
		if (NT == "+" || NT == "-"){
			while (ope.top() == 3 || ope.top() == 4){
				int t = ope.top(); ope.pop();
				int t1 = num.top();
				num.pop();
				int t2 = num.top();
				num.pop();
				if (t == 3)num.push(t1*t2);
				if (t == 4)num.push(t1/t2);
			}
		}
		
			
	}
	var render(){
		if (NT == "if"){

		}
		else if (NT == "while"){
			
		}
		else if (NT == "num"){

		}
		else if (NT == "str"){

		}
		else if (isVar() && word_ori[render_now +1 ] == "="){
			//set varibles
			var ret = render();
			
		}
		else{
			//calculate expression
			calExp();
		}
	}
};
class word_controller{
public:
	char c, lc;
	string last_type;
	string last_word;
	void last_save(){
		word_now++;
		word_ori[word_now] = last_word;
		if (last_word == "if")word_type[word_now] = "v_if";
		else if (last_word == "else")word_type[word_now] = "v_else";
		else if (last_word == "while")word_type[word_now] = "v_while";
		else if (last_word == "num")word_type[word_now] = "v_num";
		else if (last_word == "str")word_type[word_now] = "v_str";
		else word_type[word_now] = last_type;
		last_type = "";
		last_word = "";
	}
	bool sys_preprocess(){
		/*
		bool onCond = 0,onCondBlock = 0;
		for (int i = 1; i <= word_now; i++){
			if (word_ori[i] == "v_if" && word_ori[i + 1] == "("){
				word_type[i] = "func_name";
				word_type[i + 1] = "func_cond_begin";
				onCond = 1;
			}
			if (onCond && word_ori[i] == ")"){
				word_type[i] = "func_cond_end";
				onCond = 0;
				if (word_ori[i + 1] == "block_begin"){
					onCondBlock = 1;
					word_type[i + 1] == "cond_block_begin";
				}
			}
		}
		*/
		for (int i = 1; i <= word_now; i++){
			if (NT == "operator "){
				if (NO == "+")NT = "+";
				if (NO == "-")NT = "-";
				if (NO == "*")NT = "*";
				if (NO == "/")NT = "/";
				if (NO == "(")NT = "(";
				if (NO == ")")NT = ")";

			}
			

		}
	}
	bool sys_input(){
		char c, lc=' '; int n = 0; string str;
		c = getchar();
		word_now = 0; int onBlock = 0;
		string wordsaver = "";
		while (c != ';' && onBlock ){
			if (isNum(c)){
				if (last_type=="number"){
					last_word += c;
				}
				else{
					last_save();
					last_type = "number";
					last_word = c;
				}
			}
			else if (isLetter(c)){
				if (last_type == "letter"){
					last_word += c;
				}
				else{
					last_save();
					//last_type = IS_LET;
					last_type = "letter";
					last_word = c;
				}
			}
			else if (c == '{' || c == '}'){
				last_save(); last_word = c;
				if (c == '{'){
					onBlock = 1;
					last_type = "block_begin";
					
				}else{
					onBlock = 0;
					last_type = "block_end";
				}
			}
			else{
				if (last_type == "operator"){
					last_word += c;
				}
				else{
					last_save();
					last_type = "operator";
					last_word = c;
				}
			}
			if (!(c = getchar()))return 0;
		}
		for (int i = 1; i <= word_now; i++){
			cout << word_ori[i] << " " << word_type[i] << endl;
		}
	}
}ins;

int main(){
	while (ins.sys_input()){
		ins.sys_preprocess();
	}
	return 0;
}