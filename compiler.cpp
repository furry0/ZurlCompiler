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
class sys_class{
public:
	int stoi(string x){
		int ret = 0;
		for (int i = 0; i <= x.length() - 1; i++){
			ret = ret * 10 + x[i] - '0';
		}
		return ret;
	}
	string itos(int x){
		string ret;
		int flag = 0;
		if (x < 0){ flag = 1; x = -x; }
		if (x == 0)return "0";
		while (x){
			ret = char(x % 10 + '0') + ret;
			x /= 10;
		}
		if (flag)return "-" + ret;
		return ret;
	}
}sys;
class var{
	public:
		bool type;
		int i;
		string* s;
		var(){

		}
		var(int x){
			type = 0;
			i = x;
		}
		var(string x){
			type = 1;
			s = new string(x);
		}
		var operator+(const var t)const{
			if (type == 0 && t.type == 0){
				//both int
				return var(i + t.i);
			}
			else if (type == 0 && t.type == 1){
				return var(sys.itos(i) + (*t.s));
			}
			else if (type == 1 && t.type == 0){
				return var(*s + sys.itos(t.i));
			}
			else if (type == 1 && t.type == 1){
				return var(*s + *(t.s));
			}
		}
		var operator-(const var t)const{
			if (type == 0 && t.type == 0){
				//both int
				return var(i - t.i);
			}
			else {
				return var("ERROR");
			}
		}
		var operator*(const var t)const{
			if (type == 0 && t.type == 0){
				//both int
				return var(i * t.i);
			}
			else if (type == 0 && t.type == 1){
				var ret = var("");
				for (int j = 1; j <= t.i; j++)
					*(ret.s) += *(s);
				return ret;
			}
			else {
				return var("ERROR");
			}
		}
		var operator/(const var t)const{
			if (type == 0 && t.type == 0){
				//both int
				return var(i / t.i);
			}
			else {
				return var("ERROR");
			}
		}
		string out(){
			if (type == 0)return sys.itos(i);
			else if (type == 1)return *s;
		}
};

class excute_controller{
public:
	map<string, var> global_var;
	bool initrender(){
		render_now = 0;
	}
	bool isVar(){
		if (global_var.empty())return 0;
		if (NT == "letter" && global_var.find(NO) != global_var.end()){
			return 1;
		}
			
	}
	bool isVar(int x){
		if (global_var.empty())return 0;
		if (word_type[x] == "letter" && global_var.find(word_ori[x]) != global_var.end()){
			return 1;
		}

	}
	bool getVarType(){
		//must exist
		return global_var.find(NO)->second.type;
	}
	var calExp(){
		//int version
		stack<int> ope;
		stack<var> num;
		bool last = 1;
		while (NT != ";"){
			// cout<<NT<<"^"<<NO<<endl;
			if (NT == "number"){
				num.push(var(sys.stoi(NO))); last = 0;
			}
			if (NT == "str"){
				num.push(var(NO));
				last = 0;
			}
			if (NT == "letter" && isVar()){
				//not judge string
				//cout << "A VAR !!!" << endl;
				num.push(global_var.find(NO)->second);
				last = 0;
			}
			if (NT == "{"){
				num.push(render()); last = 0;
		    }
			if (NT == "+" || NT == "-"){
				if (last == 1){
					num.push(var(0));
				}
				while (!ope.empty() && (ope.top() == 3 || ope.top() == 4)){
					int t = ope.top(); ope.pop();
					var t1 = num.top();
					num.pop();
					var t2 = num.top();
					num.pop();
					if (t == 3)num.push(t2*t1);
					if (t == 4)num.push(t2/t1);
				}
				if(NT == "+")ope.push(1);
				if(NT == "-")ope.push(2);
			}
			if (NT == "*" || NT == "/"){
				if(NT =="*")ope.push(3);
				if(NT == "/")ope.push(4);
				last = 1;
			}
			if(NT == "("){
				ope.push(5);
				last = 1;
			}
			if(NT == ")"){
				while ( !ope.empty() && ope.top() != 5 ){
					int t = ope.top(); ope.pop();
					var t1 = num.top();
					num.pop();
					var t2 = num.top();
					num.pop();
					if (t == 1)num.push(t2 + t1);
					if (t == 2)num.push(t2 - t1);
					if (t == 3)num.push(t2 * t1);
					if (t == 4)num.push(t2 / t1);
				}
				if (ope.empty())return "ERROR: blacket";
				ope.pop();
				last = 0;
			}	
			render_now++;
		}
		while (!ope.empty()){
			int t = ope.top(); ope.pop();
			var t1 = num.top();
			num.pop();
			var t2 = num.top();
			num.pop();
			if (t == 1)num.push(t2 + t1);
			if (t == 2)num.push(t2 - t1);
			if (t == 3)num.push(t2 * t1);
			if (t == 4)num.push(t2 / t1);
		}
		return num.top();
	}
	bool make_var(string name,bool type){
		if (type){
			global_var.insert(pair<string, var>(name, var("")));
		}
		else{
			global_var.insert(pair<string, var>(name, var(0)));		
		}
		return 1;
	}
	void render_noeff(){
		int onBrace = 0;
		while (NT != ";" || onBrace){
			if (NT == "{")onBrace++;
			if (NT == "}")onBrace--;
			render_now++;
		}
	}
	var render(){
		if (NT == "din"){
			if (word_type[render_now + 3] == ";" ){
				if (!isVar(render_now + 2)){
					return var("Error :THE VARIBLES iS NOT EXIST");
				}
				if (word_type[render_now + 1] == "dnum" ){
					var &tvar = global_var.find(word_ori[render_now + 2])->second;
					if (tvar.type == 0){
						cin>>tvar.i;
						return var("");
					}
					else{
						return var("Error: syntax error");
					}			
				}
				else if(word_type[render_now + 1] == "dstr"){
					var &tvar = global_var.find(word_ori[render_now + 2])->second;
					if (tvar.type == 1){
						cin >> *tvar.s;
						return var("");
					}
					else{
						return var("Error: syntax error");
					}
				}
				else {

				}
			}
			else{
				return var("Error: syntax error");
			}
		}
		else if (NT == "dif"){
			var ret = render();
			if (ret.type == 1){
				return var("if not support str");
			}
			else{
				if (ret.i == 0){
					render_noeff();
					if (NT == "delse")return render();
					else return var(0);
					
				}
				else{
					return render();
				}
			}
		}
		else if (NT == "dwhile"){
			
		}
		else if (NT == "dnum"){
			if (word_type[render_now + 1] == "letter" && word_type[render_now + 2] == ";"){
				make_var(word_ori[render_now + 1],0);
				render_now += 2;
				return var(0);
			}
			else{
				cout << "ERROR:NUM LETTER" << endl;
				
			}
		}
		else if (NT == "dstr"){
			if (word_type[render_now + 1] == "letter" && word_type[render_now + 2] == ";"){
				make_var(word_ori[render_now + 1], 1);
				render_now += 2;
				return var("");
			}
			else{
				cout << "ERROR:NUM LETTER" << endl;

			}
		}
		else if (isVar() && word_ori[render_now +1 ] == "="){
			var &tvar = global_var.find(NO)->second;
			render_now += 2;
			//set varibles
			var ret = render();
			
			if (ret.type == tvar.type){
				if (ret.type == 0)tvar.i = ret.i;
				else *(tvar.s) = *(ret.s);
			}
			else{
				cout << "TYPE NOT OK" << endl;
			}
		}
		else if (NT == "{"){
			var ret;
			while (render_now <= word_now && word_type[render_now + 1] != "}"){
				render_now++;
				ret = render();
			}
			render_now++;
			return ret;
		}else {
			//calculate expression
			return calExp();
		}
	}
}exe;
class word_controller{
public:
	char c, lc;
	string last_type;
	string last_word;
	void last_save(){
		if (last_type == "")return;
		word_now++;
		word_ori[word_now] = last_word;
		if (last_word == "if")word_type[word_now] = "dif";
		else if (last_word == "else")word_type[word_now] = "delse";
		else if (last_word == "while")word_type[word_now] = "dwhile";
		else if (last_word == "num")word_type[word_now] = "dnum";
		else if (last_word == "str")word_type[word_now] = "dstr"; 
		else if (last_word == "in")word_type[word_now] = "din";
		else if (last_word == "out")word_type[word_now] = "dout";
		else if (last_word == "+")word_type[word_now] = "+";
		else if (last_word == "-")word_type[word_now] = "-";
		else if (last_word == "*")word_type[word_now] = "*";
		else if (last_word == "/")word_type[word_now] = "/";
		else if (last_word == "(")word_type[word_now] = "(";
		else if (last_word == ")")word_type[word_now] = ")";
		else if (last_word == "{")word_type[word_now] = "{";
		else if (last_word == "}")word_type[word_now] = "}";
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
			
		}
		return 1;
	}
	bool sys_input(){
		last_type = "";
		char c, lc=' '; int n = 0; string str;
		c = getchar();
		word_now = 0; int onBlock = 0,onString =0;
		string wordsaver = "";
		while (c != ';' || onBlock || onString){
			if( c== ';'){
				last_save();
				last_type = ";";
			}
			else if (onString && c != '"'){
				last_word += c;
			}else if (isNum(c)){
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
			else if (c == '"'){
				if (onString){
					last_save();
					onString = 0;
				}
				else{
					last_save();
					onString = 1;
					last_type = "str";
					
				}
			}
			else if (c == '{' || c == '}' || c==')' || c=='(' ){
				last_save(); last_word = c;
				if (c == '('){
						last_type = "b_begin";
				}
				else if (c == ')'){
						last_type = "b_end";
				
				}else if (c == '{'){
					onBlock = 1;
					last_type = "block_begin";
				}else{
					onBlock = 0;
					last_type = "block_end";
				}
			}
			else if ( c == ' ' || c == '\n' ){
				last_save();
				last_type = "";
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
		last_save();
		word_now++;
		word_type[word_now] = ";";
	}
}ins;

int main(){
	while (ins.sys_input()){
		ins.sys_preprocess();
		/*for (int i = 1; i <= word_now; i++){
			cout << word_ori[i] << " " << word_type[i] << endl;
		}*/
		render_now = 1;
		cout << exe.render().out() << endl;
		
	}
	return 0;
}
