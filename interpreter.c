#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

//shoudlve used a fucking object oriented language, wouldve made a lot simpler cleaner code 
//with polymorphisms, encapsulation and inhertiance (especially)

struct Storage{
	int value;
	char* name;
	struct Storage* next;
};


struct BinOpNode{
	struct BinOpNode* leftvalue;
	struct BinOpNode* rightvalue;
	int binOpType; //0=number,1=variable,2=plus,3=minus,4=times,5=div,6=assign,7=GTHAN,8=LTHAN,9=GTHANE,10=LTHANE,11=AND,12=OR,13=EEQUALS, 14=IF
	char* numberStorage; //for number node, stores the number 
	char* alphaStorage; //for variable node, stores variable name
};

struct VariableStack{
	struct VariableStack* next;
	char* varName;
	char* varValue;
	void* memAddress;
};

struct PostfixStack{
	struct PostfixStack* next;
	struct PostfixStack* prev;
	char* value;
};

struct EvalStack{
	struct EvalStack* next;
	char* value;
};

struct AsmRegisters{
	struct AsmRegisters* next;
	char* reg;
};



bool checkCharIsNum(char* line, int index);
bool checkCharIsAlpha(char* line, int index);
char* constructNumber(char* line,int index,char* accumulator);
char* constructAlpha(char* line,int index,char* accumulator);
int lastDigitIndex(char* line, int index);
int lastAlphaIndex(char* line, int index);
void insertTokens(char* line, int index, char* tokenList[1000], int i);
void generateTokenList(char* line,char* tokenList[1000]);
int lengthLexicon(char* tokenList[1000], int i);
void constructList(char* tokenList[1000],char* newList[1000],int startIndex,int endIndex);
int rightMostBinop(char* tokenList[1000],char* binop);
struct BinOpNode* A(char* tokenList[]);
struct BinOpNode* C(char* tokenList[]);
struct BinOpNode* B(char* tokenList[]);
struct BinOpNode* F(char* tokenList[]);
struct BinOpNode* T(char* tokenList[]);
struct BinOpNode* E(char* tokenList[]);
void printAST(struct BinOpNode* node);
char* fetchValueByName(struct VariableStack* storage, char* varname);
void reassignVariable(struct VariableStack* storage, char* varname, char* value);
void assignVariable(struct VariableStack* storage, char* varname, char* value);
char* PostfixPEEK(struct PostfixStack* stack);
char* PostfixPOP(struct PostfixStack* stack);
struct PostfixStack* PostfixPUSH(struct PostfixStack* stack, char* val);
void PostfixAPPEND(struct PostfixStack* stack, char* val);
struct PostfixStack* generatePostfix(struct BinOpNode* binop,struct PostfixStack* stack,bool isLvalue);
char* EvalPOP(struct EvalStack* stack);
struct EvalStack* EvalPUSH(struct EvalStack* stack, char* val);
int lengthString(char* string,int i);
int stoi(char* string,int i,int j);
struct EvalStack* compileOp(char* operation, struct EvalStack* stack, struct VariableStack* storage);


int main(){
	char* line = "new2=54*42+89>=3*5+60+8";
	char* test[1000];
	generateTokenList(line,test);
	struct BinOpNode* t = A(test);
	printAST(t);
	printf("\n");


	char* line2 = "5>89*9";
	char* test2[1000];
	generateTokenList(line2,test2);
	struct BinOpNode* t2 = A(test2);
	printAST(t2);
	printf("\n");


	char* line3 = "bool1=47+89<480&&67>=66||x+8==19*y";
	char* test3[1000];
	generateTokenList(line3,test3);
	struct BinOpNode* t3 = A(test3);
	printAST(t3);
	printf("\n");







	char* line4 = "x=b*b-4*a*c+63";
	char* test4[1000];
	generateTokenList(line4,test4);
	struct BinOpNode* t4 = A(test4);
	printAST(t4);
	printf("\n");

	struct PostfixStack* s = (struct PostfixStack*)malloc(sizeof(struct PostfixStack));
	//generatePostfix(t4,s);
	//struct PostfixStack* a = PostfixPUSH(s,"5");
	//struct PostfixStack* b = PostfixPUSH(a,"7");
	printf("\n");
	struct PostfixStack* us = generatePostfix(t4,s,false);
	struct PostfixStack* head = us;
	while (head->next != NULL){
		printf("%s,",head->value);
		head = head->next;
	}


	char* s1 = "57";
	char* s2 = "3";
	int r = atoi(s1) + atoi(s2);
	char result[20];
	printf("\n");
	sprintf(result,"%d",r);
	char* v = result;
	printf("%s",v);







	printf("\n");
	printf("\n");

	char* line5 = "476+38*43-1/6+7";
	char* test5[1000];
	generateTokenList(line5,test5);
	struct BinOpNode* t5 = A(test5);
	printAST(t5);
	printf("\n");






	struct VariableStack* v8 = (struct VariableStack*)malloc(sizeof(struct VariableStack));
	struct EvalStack* s9 = (struct EvalStack*)malloc(sizeof(struct EvalStack));
	struct EvalStack* s10 = compileOp("5",s9,v8);
	struct EvalStack* s11 = compileOp("6",s10,v8);


	printf("%s\n",s11->value);
	printf("%s\n",s11->next->value);

	struct EvalStack* s12 = compileOp("+",s11,v8);


	printf("test%s\n",s12->value);

	return 0;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//LEXER


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



bool checkCharIsNum(char* line, int index){
	char currentChar = *(line+index);
	bool isNum = false;
	if (currentChar <= 57 && currentChar >= 48){ //ascii 48 is 0 and ascii 57 is 9 so between these or if them then its a digit 0-9
		isNum = true;
	}
	return isNum;	
}

bool checkCharIsAlpha(char* line, int index){
	char currentChar = *(line+index);
	char* alpha = "abcdefghijklmnopqrstuvwxyz";
	bool isNum = false;
	for (int i = 0; i < 26; i++){
		if (currentChar == *(alpha+i)){
			isNum = true;
		}
	}
	return isNum;
}


char* constructNumber(char* line,int index,char* accumulator){
	bool isNum = checkCharIsNum(line,index);
	if (isNum == true){
		char currentChar = *(line+index);
		return constructNumber(line,index+1,strncat(accumulator,&currentChar,1));
	}else{
		//printf("%s\n",accumulator);
		return accumulator;
	}
	return 0;
}

char* constructAlpha(char* line,int index,char* accumulator){
	bool isNum = (checkCharIsNum(line,index) || checkCharIsAlpha(line,index));
	//printf("%d\n",isNum);
	if (isNum == true){
		char currentChar = *(line+index);
		return constructAlpha(line,index+1,strncat(accumulator,&currentChar,1));
	}else{
		return accumulator;
	}
	return 0;
}



int lastDigitIndex(char* line, int index){
	bool isNum = checkCharIsNum(line,index);
	if (isNum == true){
		return lastDigitIndex(line,index+1);
	}else{
		//printf("%d\n",index-1);
		return index-1;
	}
}

int lastAlphaIndex(char* line, int index){
	bool isNum = (checkCharIsAlpha(line,index) || checkCharIsNum(line,index));
	if (isNum == true){
		return lastAlphaIndex(line,index+1);
	}else{
		return index-1;
	}
}


void insertTokens(char* line, int index, char* tokenList[1000], int i){
	char currentChar = *(line+index);
	if (currentChar == '+'){
		*(tokenList+i)="PLUS";
		insertTokens(line,index+1,tokenList,i+1);
	}else if(currentChar == '-'){
		*(tokenList+i)="MINUS";
		insertTokens(line,index+1,tokenList,i+1);
	}else if(currentChar == '*'){
		*(tokenList+i)="TIMES";
		insertTokens(line,index+1,tokenList,i+1);
	}else if(currentChar == '/'){
		*(tokenList+i)="DIV";
		insertTokens(line,index+1,tokenList,i+1);
	}else if(currentChar == '('){
		*(tokenList+i)="LPAREN";
		insertTokens(line,index+1,tokenList,i+1);
	}else if(currentChar == ')'){
		*(tokenList+i)="RPAREN";
		insertTokens(line,index+1,tokenList,i+1);
	}else if(currentChar == '=' && *(line+index+1) != '='){
		*(tokenList+i)="ASSIGN";
		insertTokens(line,index+1,tokenList,i+1);
	}else if(currentChar == '>' && *(line+index+1) != '='){
		*(tokenList+i)="GTHAN";
		insertTokens(line,index+1,tokenList,i+1);
	}else if(currentChar == '>' && *(line+index+1) == '='){
		*(tokenList+i)="GTHANE";
		insertTokens(line,index+2,tokenList,i+1);
	}else if(currentChar == '<' && *(line+index+1) != '='){
		*(tokenList+i)="LTHAN";
		insertTokens(line,index+1,tokenList,i+1);
	}else if(currentChar == '<' && *(line+index+1) == '='){
		*(tokenList+i)="LTHANE";
		insertTokens(line,index+2,tokenList,i+1);
	}else if(currentChar == '|' && *(line+index+1) == '|'){
		*(tokenList+i)="OR";
		insertTokens(line,index+2,tokenList,i+1);
	}else if(currentChar == '&' && *(line+index+1) == '&'){
		*(tokenList+i)="AND";
		insertTokens(line,index+2,tokenList,i+1);
	}else if(currentChar == '=' && *(line+index+1) == '='){
		*(tokenList+i)="EEQUALS";
		insertTokens(line,index+2,tokenList,i+1);
	}else if(currentChar == 'i' && *(line+index+1) == 'f'){
		*(tokenList+i)="IF";
		insertTokens(line,index+2,tokenList,i+1);
	}else if(checkCharIsNum(line,index) == true){
		//printf("true\n");
		char* number = constructNumber(line,index,(char*)malloc(50));  //50 is arbitrary, might come back and tie allocated size to size of line
		*(tokenList+i)=number;
		int updatedIndex = lastDigitIndex(line,index);
		insertTokens(line,updatedIndex+1,tokenList,i+1);
	}else if(checkCharIsAlpha(line,index) == true){
		char* variable = constructAlpha(line,index,(char*)malloc(50));
		*(tokenList+i)=variable;
		int updatedIndex = lastAlphaIndex(line,index);
		insertTokens(line,updatedIndex+1,tokenList,i+1);
	}else if (currentChar == '\0'){
		*(tokenList+i)="-1"; 
	}else{
		insertTokens(line,index+1,tokenList,i+1);
	}
}

//(struct Token**)malloc(sizeof(struct Token*)*100)

void generateTokenList(char* line,char* tokenList[1000]){
	insertTokens(line,0,tokenList,0);
}



int lengthLexicon(char* tokenList[1000],int i){
	if (strcmp(*(tokenList+i),"-1")==0){
		return 0;
	}else{
		return 1+lengthLexicon(tokenList,i+1);
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



//PARSER 

//still need to implement negation -(E), binary negation !(C), and parenthesis (A)
//grammar rules


//A:=C|F=C
//C:=B|C||B|C&&B
//B:=E|E>E|E<E|E>=E|E<=E|E==E
//E:=T|E+F|E-F
//T:=F|T*F|T/F
//F:=<var>|<const>


// ex. E: if rightmost index for + in line is greater than rightmost index for - in line, then do + first then -, if do this then E+T works 
// implemented succsessfully ^ 


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void constructList(char* tokenList[1000],char* newList[1000],int startIndex,int endIndex){
	int j = 0;
	for (int i = 0; i < 1000; i++){
		if (i >= startIndex && i < endIndex){
			*(newList+j) = *(tokenList+i);
			j++;
		}else if (i == endIndex){
			*(newList+j) = *(tokenList+i);
			*(newList+j+1) = "-1";	
			break;
		}
	}
}



int rightMostBinop(char* tokenList[1000],char* binop){
	int intbinopIndicesList[1000];
	int listLen = lengthLexicon(tokenList,0);
	int j = 0;
	for (int i = 0; i < listLen; i++){ //go back and replace all instances of n=1000 and use lengthLexicon function to optimize/minimize memory allocation sizes
		if (strcmp(*(tokenList+i),binop) == 0){
			*(intbinopIndicesList+j) = i;
			j++;;
		}
	} 
	if (j > 0){
		int lastIndex = *(intbinopIndicesList+j-1);
		return lastIndex;
	}else{
		return -1;
	}

}



struct BinOpNode* If(char* tokenList[]){
	if (lengthLexicon(tokenList,0) == 1){
		return F(tokenList);
	}	
	int indexIf = rightMostBinop(tokenList,"IF");
	if (indexIf == -1){
		return C(tokenList);
	}else{
		struct BinOpNode* newNode = (struct BinOpNode*)malloc(sizeof(struct BinOpNode));
		char* leftList[1000];
		char* rightList[1000];
		constructList(tokenList,leftList,0,indexIf-1);
		constructList(tokenList,rightList,indexIf+1,lengthLexicon(tokenList,0)-1);
		struct BinOpNode* leftval = A(leftList); //F
		struct BinOpNode* rightval = C(rightList); //E(rightList)
		newNode->leftvalue = leftval;
		newNode->rightvalue = rightval;
		newNode->binOpType = 14;
		return newNode;			
	}
}



struct BinOpNode* A(char* tokenList[]){
	if (lengthLexicon(tokenList,0) == 1){
		return F(tokenList);
	}
	int indexRightMost = rightMostBinop(tokenList,"ASSIGN");
	if (indexRightMost == -1){
		return C(tokenList); //B(tokenlist)
	}else{
		struct BinOpNode* newNode = (struct BinOpNode*)malloc(sizeof(struct BinOpNode));
		char* leftList[1000];
		char* rightList[1000];
		constructList(tokenList,leftList,0,indexRightMost-1);
		constructList(tokenList,rightList,indexRightMost+1,lengthLexicon(tokenList,0)-1);
		struct BinOpNode* leftval = F(leftList); //F
		struct BinOpNode* rightval = C(rightList); //E(rightList)
		newNode->leftvalue = leftval;
		newNode->rightvalue = rightval;
		newNode->binOpType = 6;
		return newNode;	
	}
}

struct BinOpNode* C(char* tokenList[]){
	if (lengthLexicon(tokenList,0) == 1){
		return F(tokenList);
	}else{
		int indexRightMost = -1;
		int indexRightMostAnd = rightMostBinop(tokenList,"AND");
		int indexRightMostOr = rightMostBinop(tokenList,"OR");
		int type = -1;

		if (indexRightMostAnd == -1 && indexRightMostOr == -1){
			return B(tokenList);
		}else if (indexRightMostAnd > indexRightMostOr){
			indexRightMost = indexRightMostAnd;
			type = 11;
		}else{
			indexRightMost = indexRightMostOr;
			type = 12;
		}

		struct BinOpNode* newNode = (struct BinOpNode*)malloc(sizeof(struct BinOpNode));
		char* leftList[1000];
		char* rightList[1000];
		constructList(tokenList,leftList,0,indexRightMost-1);
		constructList(tokenList,rightList,indexRightMost+1,lengthLexicon(tokenList,0)-1);
		struct BinOpNode* leftval = C(leftList);
		struct BinOpNode* rightval = B(rightList);  //make sure this is correct
		newNode->leftvalue = leftval;
		newNode->rightvalue = rightval;
		newNode->binOpType = type;
		return newNode;
	}

}

struct BinOpNode* B(char* tokenList[]){
	if (lengthLexicon(tokenList,0) == 1){
		return F(tokenList);
	}
	int indexRightMost = rightMostBinop(tokenList,"GTHANE");
	int type = -1;
	if (indexRightMost == -1){
		indexRightMost = rightMostBinop(tokenList,"GTHAN");
		if (indexRightMost == -1){
			indexRightMost = rightMostBinop(tokenList,"LTHANE");
			if (indexRightMost == -1){
				indexRightMost = rightMostBinop(tokenList,"LTHAN");
				if (indexRightMost == -1){
					indexRightMost = rightMostBinop(tokenList,"EEQUALS");
					if (indexRightMost == -1){
						return E(tokenList);
					}else{
						type = 13;
					}
				}else{
					type = 8;
				}
			}else{
				type = 10;
			}
		}else{
			type = 7;
		}
	}else{
		type = 9;
	}
	struct BinOpNode* newNode = (struct BinOpNode*)malloc(sizeof(struct BinOpNode));
	char* leftList[1000];
	char* rightList[1000];
	constructList(tokenList,leftList,0,indexRightMost-1);
	constructList(tokenList,rightList,indexRightMost+1,lengthLexicon(tokenList,0)-1);
	struct BinOpNode* leftval = E(leftList);
	struct BinOpNode* rightval = E(rightList); 
	newNode->leftvalue = leftval;
	newNode->rightvalue = rightval;
	newNode->binOpType = type;
	return newNode;	
}


struct BinOpNode* F(char* tokenList[]){
	struct BinOpNode* newNode = (struct BinOpNode*)malloc(sizeof(struct BinOpNode));
	newNode->leftvalue = NULL;
	newNode->rightvalue = NULL;
	newNode->binOpType = 0;
	if (checkCharIsAlpha(*tokenList,0) == true){
		newNode->alphaStorage = *tokenList;
	}else if (checkCharIsNum(*tokenList,0) == true){
		newNode->numberStorage = *tokenList;
	}
	return newNode;
}



struct BinOpNode* T(char* tokenList[]){
	if (lengthLexicon(tokenList,0) == 1){
		return F(tokenList);
	}else{
		int indexRightMost = -1;
		int indexRightMostTimes = rightMostBinop(tokenList,"TIMES");
		int indexRightMostDiv = rightMostBinop(tokenList,"DIV");
		int type = -1;

		if (indexRightMostTimes == -1 && indexRightMostDiv == -1){
			return F(tokenList);
		}else if (indexRightMostTimes > indexRightMostDiv){
			indexRightMost = indexRightMostTimes;
			type = 4;
		}else{
			indexRightMost = indexRightMostDiv;
			type = 5;
		}

		struct BinOpNode* newNode = (struct BinOpNode*)malloc(sizeof(struct BinOpNode));
		char* leftList[1000];
		char* rightList[1000];
		constructList(tokenList,leftList,0,indexRightMost-1);
		constructList(tokenList,rightList,indexRightMost+1,lengthLexicon(tokenList,0)-1);
		struct BinOpNode* leftval = T(leftList);
		struct BinOpNode* rightval = F(rightList);
		newNode->leftvalue = leftval;
		newNode->rightvalue = rightval;
		newNode->binOpType = type;
		return newNode;
	}
}




struct BinOpNode* E(char* tokenList[]){
	if(lengthLexicon(tokenList,0) == 1){
		return F(tokenList);
	}else{
		int indexRightMost = -1;
		int indexRightMostPlus = rightMostBinop(tokenList,"PLUS");
		int indexRightMostMinus = rightMostBinop(tokenList,"MINUS");
		int type = -1;

		if (indexRightMostPlus == -1 && indexRightMostMinus == -1){ //if plus or minus ops dont exist in list, send to next op precedence (T)
			return T(tokenList);
		}else if (indexRightMostPlus > indexRightMostMinus){ //right most plus index > right most minus index then split list/choose E+T at right most +
			//index in order that right list after split has no plusses or minuses so right list can be sent to T
			indexRightMost = indexRightMostPlus;
			type = 2;
		}else{ //right most minus index > right most plus index or they are equal, E-T, send right list to T, left list contains all plus/minuses
			indexRightMost = indexRightMostMinus;
			type = 3;
		}

		struct BinOpNode* newNode = (struct BinOpNode*)malloc(sizeof(struct BinOpNode));
		char* leftList[1000];
		char* rightList[1000];
		constructList(tokenList,leftList,0,indexRightMost-1);
		constructList(tokenList,rightList,indexRightMost+1,lengthLexicon(tokenList,0)-1);
		struct BinOpNode* leftval = E(leftList);//E(leftList);
		struct BinOpNode* rightval = T(rightList);//T(rightList);
		newNode->leftvalue = leftval;
		newNode->rightvalue = rightval;
		newNode->binOpType = type;
		return newNode;	
	}
}





void printAST(struct BinOpNode* node){
	if (node->binOpType == 0){
		if (node->numberStorage == NULL){
			printf("%s",node->alphaStorage);
		}else{
			printf("%s",node->numberStorage);
		}
	}else{
		printf("(");
		printAST(node->leftvalue);
		if (node->binOpType == 2){
			printf("+");
		}else if(node->binOpType == 3){
			printf("-");
		}else if(node->binOpType == 4){
			printf("*");
		}else if(node->binOpType == 5){
			printf("/");
		}else if(node->binOpType == 6){
			printf("=");
		}else if(node->binOpType == 7){
			printf(">");
		}else if(node->binOpType == 8){
			printf("<");
		}else if(node->binOpType == 9){
			printf(">=");
		}else if(node->binOpType == 10){
			printf("<=");
		}else if(node->binOpType == 11){
			printf("&&");
		}else if(node->binOpType == 12){
			printf("||");
		}else if(node->binOpType == 13){
			printf("==");
		}else if(node->binOpType == 14){
			printf("if ");
		}
		printAST(node->rightvalue);
		printf(")");
	}
}




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//VARIABLE STACK COMMANDS


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



char* fetchValueByName(struct VariableStack* storage, char* varname){
	if (storage == NULL){
		return NULL;
	}else if(storage->varName == varname){
		return storage->varValue;
	}else{
		return fetchValueByName(storage->next,varname);
	}
}

void reassignVariable(struct VariableStack* storage, char* varname, char* value){
	if (storage == NULL){
		printf("Variable '%s' has not been assigned yet",varname);
	}else if(storage->varName == varname){
		storage->varValue = value;
	}else{
		reassignVariable(storage->next,varname,value);
	}
}

void assignVariable(struct VariableStack* storage, char* varname, char* value){
	if (storage->next == NULL){
		struct VariableStack* newVariable = (struct VariableStack*)malloc(sizeof(struct VariableStack));
		newVariable->varValue = value;
		newVariable->varName = varname;
		storage->next = newVariable;
	}else{
		assignVariable(storage->next,varname,value);
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//POSTFIXSTACK COMMANDS


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

char* PostfixPEEK(struct PostfixStack* stack){
	return stack->value;
}

char* PostfixPOP(struct PostfixStack* stack){
	struct PostfixStack* head = stack;
	head->next = NULL;
	stack = stack->next;
	return head->value;
}

struct PostfixStack* PostfixPUSH(struct PostfixStack* stack, char* val){
	struct PostfixStack* newItem = (struct PostfixStack*)malloc(sizeof(struct PostfixStack));
	newItem->next = stack;
	newItem->value = val;
	//stack = newItem; //dont know why this doesnt work as its a pointer and reassigning it to address of new struct
	return newItem;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//POSTFIX GENERATION
//0=number,1=variable,2=plus,3=minus,4=times,5=div,6=assign,7=GTHAN,8=LTHAN,9=GTHANE,10=LTHANE,11=AND,12=OR,13=EEQUALS, 14=IF

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



struct PostfixStack* generatePostfix(struct BinOpNode* binop,struct PostfixStack* stack,bool isLvalue){
	if (binop->binOpType == 0){
		if (binop->numberStorage == NULL){
			if (isLvalue == false){
				struct PostfixStack* updatedStack = PostfixPUSH(stack,"Fetch");
				return PostfixPUSH(updatedStack,binop->alphaStorage);
			}else{
				return PostfixPUSH(stack,binop->alphaStorage);
			}
		}else{
			return PostfixPUSH(stack,binop->numberStorage);
		}
	}else{
		struct PostfixStack* updatedStack = (struct PostfixStack*)malloc(sizeof(struct PostfixStack));
		bool lval = false;
		if (binop->binOpType == 2){ 
			updatedStack = PostfixPUSH(stack,"+");
		}else if(binop->binOpType == 3){
			updatedStack = PostfixPUSH(stack,"-");
		}else if(binop->binOpType == 4){
			updatedStack = PostfixPUSH(stack,"*");
		}else if(binop->binOpType == 5){
			updatedStack = PostfixPUSH(stack,"/");
		}else if(binop->binOpType == 6){
			updatedStack = PostfixPUSH(stack,"=");
			lval = true;
		}else if(binop->binOpType == 7){
			updatedStack = PostfixPUSH(stack,">");
		}else if(binop->binOpType == 8){
			updatedStack = PostfixPUSH(stack,"<");
		}else if(binop->binOpType == 9){
			updatedStack = PostfixPUSH(stack,">=");
		}else if(binop->binOpType == 10){
			updatedStack = PostfixPUSH(stack,"<=");
		}else if(binop->binOpType == 11){
			updatedStack = PostfixPUSH(stack,"&&");
		}else if(binop->binOpType == 12){
			updatedStack = PostfixPUSH(stack,"||");
		}else if(binop->binOpType == 13){
			updatedStack = PostfixPUSH(stack,"==");
		}else if(binop->binOpType == 14){
			updatedStack = PostfixPUSH(stack,"If");
		}
		struct PostfixStack* rightUpdatedStack = generatePostfix(binop->rightvalue,updatedStack,lval); //rightvalue first then leftvalue 
		return generatePostfix(binop->leftvalue,rightUpdatedStack,lval);
	}
}







////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//EVALSTACK EVALUATOR


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



char* EvalPOP(struct EvalStack* stack){
	//struct EvalStack* head = stack;
	//head->next = NULL;
	//stack = stack->next;
	return stack->value;
}

struct EvalStack* EvalPUSH(struct EvalStack* stack, char* val){
	struct EvalStack* newItem = (struct EvalStack*)malloc(sizeof(struct EvalStack));
	newItem->next = stack;
	newItem->value = val;
	//stack = newItem; //dont know why this doesnt work as its a pointer and reassigning it to address of new struct
	return newItem;
}

int lengthString(char* string,int i){
	char c = *(string+i);
	if (c == '\0'){
		return 0;
	}else{
		return 1 + lengthString(string,i+1);
	}
}

int stoi(char* string,int i,int j){
	char c = *(string+j);
	if (i == -1){
		return 0;
	}else{
		int ctoi = c - '0';
		return (ctoi * pow(10,i)) + stoi(string,i-1,j+1);;
	}
}






////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//POSTFIX EVALUATOR
//Not working yet, need to somehow make polymorphic
//appear polymorphic by hiding int const and char* vars in struct and have
//EvalStack be linked list w int and char* vals

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


struct EvalStack* compileOp(char* operation, struct EvalStack* stack, struct VariableStack* storage){
	if (strcmp(operation,"+") == 0){
		int rightval = atoi(EvalPOP(stack));
		int leftval = atoi(EvalPOP(stack));
		printf("inside:%d\n",rightval);
		char result[20];
		int resultInt = leftval + rightval;
		sprintf(result,"%d",resultInt);
		printf("inside2:%s\n",result);
		struct EvalStack* updatedStack = EvalPUSH(stack,result);
		printf("inside3:%s\n",updatedStack->value);
		return updatedStack;
	}else if (strcmp(operation,"*") == 0){
		int rightval = atoi(EvalPOP(stack));
		int leftval = atoi(EvalPOP(stack));
		char result[20];
		int resultInt = leftval * rightval;
		sprintf(result,"%d",resultInt);
		struct EvalStack* updatedStack = EvalPUSH(stack,result);
		return updatedStack;
	}else if (strcmp(operation,"-") == 0){
		int rightval = atoi(EvalPOP(stack));
		int leftval = atoi(EvalPOP(stack));
		char result[20];
		int resultInt = leftval - rightval;
		sprintf(result,"%d",resultInt);
		struct EvalStack* updatedStack = EvalPUSH(stack,result);
		return updatedStack;
	}else if (strcmp(operation,"/") == 0){
		int rightval = atoi(EvalPOP(stack));
		int leftval = atoi(EvalPOP(stack));
		char result[20];
		int resultInt = leftval / rightval;
		sprintf(result,"%d",resultInt);
		struct EvalStack* updatedStack = EvalPUSH(stack,result);
		return updatedStack;
	}else if (strcmp(operation,"Fetch") == 0){
		char* variable = EvalPOP(stack);
		char* value = fetchValueByName(storage,variable);
		struct EvalStack* updatedStack = EvalPUSH(stack,value);
		return updatedStack;
	}else{
		struct EvalStack* updatedStack = EvalPUSH(stack,operation);
		return updatedStack;
	}
}






////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//CODE GENERATION
//pop every node of AST onto evalStack and print appropriate assembly code, then evaluate the expression, push back onto stack and repeat.


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



//int assemblyADD(struct VariableStack* storage, struct EvalStack* stack){ //binop is last binop i.e <var|const> <binop> <var|const>
//	struct BinOpNode* left = binop->leftvalue;
//	struct BinOpNode* right = binop->rightvalue;
//	int leftval = -1;
//	int rightval = -1;
//	if (left->numberStorage == NULL){
//		leftval = left->alphaStorage;
//	}else{
//		leftval = left->numberStorage;
//	}
//
//	if (right->numberStorage == NULL){
//		rightval = right->alphaStorage;
//	}else{
//		rightval = right->numberStorage;
//	}

//	printf("mov v1 %d",leftval);
//	printf("mov v2 %d",rightval);

//}
























