#include <iostream>
#include <vector>
#include <stack>
#include <cassert>
#include <cctype>
#include <list>
#include <cmath>
using namespace std;


struct tokRslt 
{
  bool success;
    vector < string > syms;
};

tokRslt tokenize (string s)
{
  vector < string > fleep;
  struct tokRslt bleep;
  while (s.length () > 0)
    {
      switch (s[0])
	{
	case ' ':
	  s.erase (0, 1);
	  break;

	case 'T':
	  fleep.push_back ("T");
	  s.erase (0, 1);
	  break;

	case 'F':
	  fleep.push_back ("F");
	  s.erase (0, 1);
	  break;

	case '^':
	  fleep.push_back ("^");
	  s.erase (0, 1);
	  break;

	case 'v':
	  fleep.push_back ("v");
	  s.erase (0, 1);
	  break;

	case '~':
	  fleep.push_back ("~");
	  s.erase (0, 1);
	  break;

	case '=':
	  if (s.length () >= 2 && s[1] == '>')
	    {
	      fleep.push_back ("=>");
	      s.erase (0, 2);
	      break;
	    }
	  else
	    {
	      bleep.success = false;
	      return bleep;
	    }
	case '<':
	  if (s.length () >= 3 && s[1] == '=' && s[2] == '>')
	    {
	      fleep.push_back ("<=>");
	      s.erase (0, 3);
	      break;
	    }
	  else
	    {
	      bleep.success = false;
	      return bleep;
	    }
	case ',':
	  fleep.push_back(",");
	  s.erase(0, 1);
	  break;
	 case ':':
	   if (s.length() >= 2 && s[1] == '.') {
	     fleep.push_back(":.");
	     s.erase(0, 2);
	     break;
	   } else {
	     bleep.success = false;
	     return bleep;
	   }
	case '(':
	  fleep.push_back ("(");
	  s.erase (0, 1);
	  break;

	case ')':
	  fleep.push_back (")");
	  s.erase (0, 1);
	  continue;
	default:
	  if (islower(s[0])) {
	    string var = "";
	    while (islower(s[0]) && s.length() > 0) {
	      var += s[0];
	      s.erase(0, 1);
	      
	    }
	    fleep.push_back(var);
	    continue;
	    
	  }
	  bleep.success = false;
	  return bleep;
	}
    }
  bleep.syms = fleep;
  bleep.success = true;
  return bleep;

}

typedef struct AST *pNODE;
struct AST
{
  string info;
  pNODE children[2];
  
  AST() {
    info = "";
    children[0] = nullptr;
    children[1] = nullptr;
  }
  
  AST(string s) {
    //cout << "NO CHILDS " << s << endl;
    info = s;
    children[0] = nullptr;
    children[1] = nullptr;
  }
  AST(string s, AST * a) {
    //cout << "ONE CHILD " << s << endl;
    info = s;
    children[0] = a;
  }
  
  AST(string s, AST * a, AST * b) {
    //cout << "TWO CHILDS " << s << endl;
    info = s;
    children[0] = a;
    children[1] = b;
  }
  
  void setInfo(string s) {
    //cout << "SETTING INFO " << s << endl;
    info = s;
  }
  
  
};



struct parseRslt
{
  bool success;
  struct AST ast;
};

int
getOperatorPrec (string s)
{
  if (s == "~")
    return 100;
  if (s == "^")
    return 99;
  if (s == "v")
    return 98;
  if (s == "=>")
    return 97;
  if (s == "<=>")
    return 96;
  
  return -99999;
}

bool isOp (string s)
{
  if (s == "~")
    return true;
  if (s == "^")
    return true;
  if (s == "v")
    return true;
  if (s == "=>")
    return true;
  if (s == "<=>")
    return true;
  if (s == ",")
    return true;
  if (s == ":.")
    return true;
  return false;
}

bool
isLeft (string s)
{
  if (s == "^" || s == "v")
    return true;
  return false;
}


struct AST *
putIntoAST (vector < string > &foo, bool &fubar)
{
  struct AST *flangus = new struct AST();
  if (fubar)
    {
      return flangus;
    }
  if (isOp (foo.back ()))
    {
      if (foo.back () == "~")
	{
	  flangus->setInfo("~");
	  foo.pop_back ();
	  if (foo.size () < 1)
	    {
	      fubar = true;
	      return flangus;
	    }
	  flangus->children[0] = putIntoAST (foo, fubar);
	  return flangus;
	}
      else
	{
	  flangus->setInfo(foo.back());
	  foo.pop_back ();
	  if (foo.size () < 2)
	    {
	      fubar = true;
	      return flangus;
	    }
	  flangus->children[1] = putIntoAST (foo, fubar);
	  if (foo.size () < 1)
	    {
	      fubar = true;
	      return flangus;
	    }
	  flangus->children[0] = putIntoAST (foo, fubar);
	  return flangus;
	}
    }
  else
    {
      flangus->setInfo(foo.back());
      foo.pop_back ();
      return flangus;
    }
}


parseRslt parse (vector < string > V)
{
  vector < string > flangus; //output queue
  stack < string > yeah;//operator stack
  int i = 0;
  
  if (V.size () == 0)
    {
      struct parseRslt bleep;
      bleep.success = false;
      return bleep;
    }
    //Shunting yard algorithm
  while (i < V.size ())
    {
      string fleep = V[i];
      if (isOp (fleep))
	{

	  while (!yeah.empty () &&
		 isOp (yeah.top ()) &&
		 (getOperatorPrec (fleep) <= getOperatorPrec (yeah.top ()) &&
		 isLeft (fleep) || getOperatorPrec(fleep) < getOperatorPrec(yeah.top())) )
	    {
	      flangus.push_back (yeah.top ());
	      yeah.pop ();
	    }
	  yeah.push (fleep);
	  i++;
	}
      else if (fleep == "(")
	{
	  yeah.push (fleep);
	  i++;
	}
      else if (fleep == ")")
	{
	  i++;
	  bool foundit = false;
	  while (!yeah.empty () && yeah.top () != "(")
	    {
	      flangus.push_back (yeah.top ());
	      yeah.pop ();
	    }
	  if (!yeah.empty () && yeah.top () == "(")
	    {
	      foundit = true;
	      yeah.pop ();
	    }

	  if (!foundit)
	    {
	      struct parseRslt qwertyuiop;
	      qwertyuiop.success = false;
	      return qwertyuiop;
	    }
	} else 
	    {
	        flangus.push_back (fleep);
	        i++;
	    }
      
    }

  while (!yeah.empty ())
    {
      string thing = yeah.top();
      if (thing == "(") {
        struct parseRslt qwertyuiop;
        qwertyuiop.success = false;
        return qwertyuiop;
      }
      flangus.push_back (yeah.top ());
      yeah.pop ();
    }
  struct parseRslt qwertyuiop;
  bool fubar = (flangus.size()==0);

  qwertyuiop.ast = *putIntoAST (flangus, fubar);
  qwertyuiop.success = !fubar && flangus.size() == 0;
  return qwertyuiop;


}


bool eval (AST T)
{
  string blarp = T.info;
  if (blarp == "~")
    {
      return !eval (*T.children[0]);
    }
  else if (blarp == "T")
    {
      return true;
    }
  else if (blarp == "F")
    {
      return false;
    }
  else if (blarp == "v")
    {
      return eval (*T.children[1]) || eval (*T.children[0]);
    }
  else if (blarp == "^")
    {
      return eval (*T.children[1]) && eval (*T.children[0]);
    }
  else if (blarp == "=>")
    {
      return !eval (*T.children[0]) || eval (*T.children[1]);
    }
  else if (blarp == "<=>")
    {
      return eval (*T.children[1]) == eval (*T.children[0]);
    }
  else
    {
      
      cout << "I HAVE NO MOUTH\n";
      cout << blarp;
      cout << "\nAND I MUST SCREAM\n";
      
      return 3;
    }
}

struct TPERslt
{
  bool val;
  string msg;
};



struct TPERslt
TPE (string s)
{
  struct TPERslt flangus;
  struct tokRslt qwer = tokenize (s);
  if (!qwer.success)
    {
      flangus.msg = "symbol error";
      return flangus;
    }
  struct parseRslt bleep = parse (qwer.syms);
  if (!bleep.success)
    {
      flangus.msg = "grammar error";
      return flangus;
    }
  flangus.msg = "success";
  flangus.val = eval (bleep.ast);
  return flangus;
}


string
TPEOut (string s)
{
  struct TPERslt flarp = TPE (s);
  if (flarp.msg == "success")
    {
      if (flarp.val)
	{
	  return "true";
	}
      else
	{
	  return "false";
	}
    }
  else
    {
      return flarp.msg;
    }
}



vector<string> clone(vector<string> thing) {
  vector<string> LString;
  LString = thing;
  return LString;
}

struct exprsResult {
  vector<vector<string>> beforeColonDot;
  vector<string> afterColonDot;
  bool success;
};

exprsResult exprs(vector<string> tokens) {
  exprsResult result;
  vector<string> currentExpr;
  bool foundColonDot = false;
  for (int i = 0; i < tokens.size(); i++) {
    if (!foundColonDot) {
      if (tokens[i] == ",") {
        result.beforeColonDot.push_back(currentExpr);
        currentExpr.clear();
      } else if (tokens[i] == ":.") {
        foundColonDot = true;
        result.beforeColonDot.push_back(currentExpr);
      } else {
        currentExpr.push_back(tokens[i]);
      }
    } else {
      if (tokens[i] == ",") {
        result.success = false;
        return result;
      } else if (tokens[i] == ":.") {
        result.success = false;
        return result;
      } else {
        result.afterColonDot.push_back(tokens[i]);
      }
    }
  }
  result.success = true;
  return result;
}


struct inference {
  vector<AST> premises;
  AST conclusion;
};

inference parseTheExprs(exprsResult exprs, bool &goodGrammar) {
  inference parsedExprs;
  for (int i = 0; i < exprs.beforeColonDot.size(); i++) {
    parseRslt flingus = parse(exprs.beforeColonDot[i]);
    if (!flingus.success) {
      goodGrammar = false;
      return parsedExprs;
    } else {
      parsedExprs.premises.push_back(flingus.ast);
    }
  }
  parseRslt fleengus = parse(exprs.afterColonDot);
  if (!fleengus.success) {
    goodGrammar = false;
    return parsedExprs;
  } else {
    parsedExprs.conclusion = fleengus.ast;
  }
  goodGrammar = true;
  return parsedExprs;
}


void Insert(string s, list<string> *L) {
  list<string>::iterator SLARK;
  for (SLARK = L->begin(); SLARK != L->end(); SLARK++) {
    if (*SLARK > s) {
      break;
    }
  }
  L->insert(SLARK, s);
  
}

void vars_(AST *T, list<string> &variables) {
  if (T->info == "~") {
    vars_(T->children[0], variables);
  } else if (T->info == "T") {}
    else if (T->info == "F") {}
    else if (T->info == "v") {
      vars_(T->children[0], variables);
      vars_(T->children[1], variables);
    } else if (T->info == "^") {
      vars_(T->children[0], variables);
      vars_(T->children[1], variables);
    } else if (T->info == "=>") {
      vars_(T->children[0], variables);
      vars_(T->children[1], variables);
    } else if (T->info == "<=>") {
      vars_(T->children[0], variables);
      vars_(T->children[1], variables);
    } else {
      variables.push_back(T->info);
    }
}

list<string> vars(AST T) {
  list<string> flangus;
  vars_(&T, flangus);
  flangus.sort();
  flangus.unique();
  return flangus;
}

list<string> vars(vector<AST> Ts) {
  list<string> flangus;
  for (int i = 0; i < Ts.size(); i++) {
    vars_(&(Ts[i]), flangus);
  }
  flangus.sort();
  flangus.unique();
  return flangus;
}


template <typename X> void printAllTheXInAList(list<X> some_string) {
  for (auto beep = some_string.begin(); beep != some_string.end(); beep++) {
    cout << *beep << " ";
  }
  cout << endl;
}


list<bool> bits(int i, int N) {
  list<bool> result;
  for (int b = 0; b < N; b++) {
    result.push_front(i%2);
      i = i / 2;
  }
  if (i!=0){
    return list <bool>();
  }
  return result;
}



AST * substitute_(list<bool> vals, list<string> vars, AST *Exp) {
  if (vals.size() != vars.size()) {
    cout << "SAME SIZE YOU MORON\n";
    exit(-1);
  }
  if (Exp->info == "~") {
      AST *only = substitute_(vals, vars, Exp->children[0]);
      return new AST("~", only);
  } else if (Exp->info == "T") {
      return Exp;
  }
    else if (Exp->info == "F") {
      return Exp;
    }
    else if (Exp->info == "v") {
      return new AST("v", substitute_(vals, vars, Exp->children[0]),  substitute_(vals, vars, Exp->children[1]));
    } else if (Exp->info == "^") {
      return new AST("^", substitute_(vals, vars, Exp->children[0]), substitute_(vals, vars, Exp->children[1]));
    } else if (Exp->info == "=>") {
      return new AST("=>", substitute_(vals, vars, Exp->children[0]), substitute_(vals, vars, Exp->children[1]));
    } else if (Exp->info == "<=>") {
      return new AST("<=>", substitute_(vals, vars, Exp->children[0]), substitute_(vals, vars, Exp->children[1]));
    } else {
      auto valsIt = vals.begin();
      for (auto varsIt = vars.begin(); varsIt != vars.end(); varsIt++, valsIt++) {
        if (Exp->info == *varsIt) {
          if (*valsIt) {
            return new AST("T");
          } else {
            return new AST("F");
          }
        }
      }
    }
  return 0;
}

AST substitute(list<bool> vals, list<string> vars, AST Exp) {
  AST * ret = substitute_(vals, vars, &Exp);
  return *ret;
}


bool subAndEval(list<bool> vals, list<string> vars, AST Exp) {
  return eval(substitute(vals, vars, Exp));
}

bool witnessInvalid(list<bool> vals, list<string> vars, inference I) {
  for (int i = 0; i < I.premises.size(); i++) {
    if (subAndEval(vals, vars, I.premises[i])) {
      continue;
    } else {
      return false;
    }
  }
  return !subAndEval(vals, vars, I.conclusion);
}

bool valid(inference I) {
  list<string> theVars = vars(I.premises);
  theVars.splice(next(theVars.end()), vars(I.conclusion));
  theVars.sort();
  theVars.unique();
  int numberOfThings = theVars.size();
  for (int valsToBeTested = 0; valsToBeTested < pow(2, numberOfThings); valsToBeTested++ ) {
    if (witnessInvalid(bits(valsToBeTested, numberOfThings), theVars, I)) {
      return false;
    }
  }
  return true;
}


string validInference(string s) {
  tokRslt toks = tokenize(s);
  if (!toks.success) {
    return "symbol error";
  }
  exprsResult beforeAndAfterColonDot = exprs(toks.syms);
  if (!beforeAndAfterColonDot.success) {
    return "grammar error";
  }
  bool correct;
  inference inf = parseTheExprs(beforeAndAfterColonDot, correct);
  if (!correct) {
    return "grammar error";
  }
  if (valid(inf)) {
    return "valid";
  } else {
    return "invalid";
  }
}

/*typedef struct AST* pNODE; 

struct AST {string info; pNODE children[2]; };

struct inference
{
  vector<AST> premises;
  AST conclusion;
};*/

void prinTree(AST T)
{
// If both children are NULL, just print the symbol
    if (T.children[0]==NULL){
    cout<< T.info;
    return;
    }

    // print an opening paren, followed by the symbol of T, followed
    // by a space, and then the first child.
    cout << "(" << T.info<<" ";
    prinTree(*(T.children[0]));
    cout << " ";

    // print the second child if there is one, and then a right paren.
    if (T.children[1] != NULL)
    prinTree(*(T.children[1]));
    cout << ")";
}

string ASTtoString(AST T) //converts an AST to String
{
	string s;
	// If both children are NULL, just print the symbol
	if (T.children[0] == NULL) {
		s = s + T.info;
		return s;
	}

	// print an opening paren, followed by the symbol of T, followed
	// by a space, and then the first child.
	s = s + "(";
	s = s + T.info;
	s = s + " ";
	s += ASTtoString(*(T.children[0]));
	s = s + " ";

	// print the second child if there is one, and then a right paren.
	if (T.children[1] != NULL) {
		s += ASTtoString(*(T.children[1]));
	}
	s = s + ")";
	return s;
}

pNODE cons(string s,pNODE c1,pNODE c2)
{
    pNODE ret = new AST;
    ret->info = s; // same as (*ret).info = s
    ret->children[0]=c1;
    ret->children[1]=c2;
    return ret;
}

//********************* TEST FUNCTIONS START HERE *****************************
void checkInsert()
{
  list<string> mylist = {"bar", "foo"};
  
  Insert("boo",&mylist); 
  
  list<string> test={"bar","boo","foo"}; 
  
  if(mylist==test) //Insert should return mylist in sorted form with new variable "boo" added
    cout<<"\nINSERT Function PASSED";
    
  else
    cout<<"\nINSERT Function FAILED";
}


//Checks list<string> vars(AST T)
void checkVars1()
{
    AST A,B,C,D,E;
    
    //Expression: p v q => T
    //Replace it with your own AST for your own test cases
    A= *cons("=>",&B,&C);
    B= *cons("v",&D,&E);
    C= *cons("T",NULL,NULL);
    D= *cons("p",NULL,NULL);
    E= *cons("q",NULL,NULL);

 
    list<string> allvars = vars(A); 
    list<string> testvars= {"p","q"} ; //List of all variables that should be in AST T, in sorted order 
    
    
    if(allvars==testvars) //Insert should return mylist in sorted form with new variable "boo" added
        cout<<"\nVars(AST T) PASSED";
        
    
    else
      cout<<"\nVars(AST T) Failed!!";
    
}

/*//Checks list<string> vars(vector<AST> Ts)
void checkVars2(inference inf)
{
  
    list<string> allvars = vars(inf.premises); 
    
    list<string> test= {"",""}   //ADD all the variables here in your premises
    test.sort();
    
    
    if(allvars==test) //Insert should return mylist in sorted form with new variable "boo" added
        cout<<"\nVars(vector<AST Ts>) PASSED";
        
    
    else
      cout<<"\nVars(vector<AST Ts>) Failed!!";
    
}*/


void checkBits()
{
  list<bool> num= {1,0,0,0,0,1,0,1,1}; //Number 267
  int i=267;
  list<bool> test1=bits(i,9);
  list<bool> test2=bits(i,5); //should return an empty list
  
  if(test1==num && test2.empty())
    cout<<"\nBits ALL test cases PASSED";
    
  else if(test2!=num)
    cout<<"\nBits failed to generate sequence for number "<<i;
    
  else
    cout<<"\nBits ALL test cases Failed";
  
}


void checkSubstitute()
{
    AST A,B,C,D,E,F;
    //AST ~p ^ q v r
    A= *cons("v",&B,&C);
    B= *cons("^",&D,&E);
    C= *cons("r",NULL,NULL);
    D= *cons("~",&F,NULL);
    E= *cons("q",NULL,NULL);
    F= *cons("p",NULL,NULL);
    
    list<bool> b={0,0,1}; // p=0 (F), q=0 (F), r=1 (T)
    list<string> v={"p","q","r"};
    
    AST result=substitute(b,v,A);
    
    string output=ASTtoString(result); //Converting to string for comparison
    
    string subAST="(v (^ (~ F ) F) T)"; //Correction Made
    
    if(output == subAST)
      cout<<"\nSubstitute ALL test case passed";
      
    else
      cout<<"\nSubstitute failed for ~p ^ q v r";
}



void  checkValidInference() 
{
   string t1="~ p v F => (ac v ~dc), ac ^ dc :. p";  //Should evaluate to Invalid
   string t2="Foo V Bar, bar <=> Foo :. T";  //symbol error
   string t3="(ab ^ cd), cd => ~ab :. ^ cd";   //grammar error
   
   
   string op1,op2,op3;
   
   op1=validInference(t1);
   op2=validInference(t2);
   op3=validInference(t3);
   
   if(op1 == "invalid" && op2 == "symbol error" && op3 == "grammar error")
     cout<<"\nValid Inference all cases passed";
    
   else if(op1!= "invalid")  
    cout<<"\n Valid Inference test case "<<t1<<" failed. EXPECTED: invalid, OUTPUT generated: "<<op1;
    
   else if(op2!= "symbol error")  
    cout<<"\n Valid Inference test case "<<t2<<" failed. EXPECTED: symbol error, OUTPUT generated: "<<op2;
    
    else if(op3!= "grammar error")  
    cout<<"\n Valid Inference test case "<<t3<<" failed. EXPECTED: grammar error, OUTPUT generated: "<<op3;
    
    else
      cout<<"\n Valid Inference test ALL test cases FAILED";
}


int main ()
{
   checkInsert();
	checkVars1();
	checkBits();
	//checkValidInference();
	//checkBits();
	checkSubstitute();
	checkValidInference();
	
	

  string testString = " ";
  tokRslt foo = tokenize(testString);
  cout << foo.success << endl;
  for (int i = 0; i < foo.syms.size(); i++) {
    cout << foo.syms[i] << endl;
  }
  exprsResult thing = exprs(foo.syms);
  thing.beforeColonDot.size();
  thing.beforeColonDot;
  for (int i = 0; i < thing.beforeColonDot.size(); i++) {
    for (int j = 0; j < thing.beforeColonDot[i].size(); j++) {
      cout << thing.beforeColonDot[i][j] << " ";
    }
    cout << endl;
  }
  for (int i = 0; i < thing.afterColonDot.size(); i++) {
    cout << thing.afterColonDot[i] << " ";
  }
  cout << endl;
  bool count;
  inference blag = parseTheExprs(thing, count);
  printAllTheXInAList(vars(blag.premises));
  printAllTheXInAList(vars(blag.conclusion));
  cout << "is blag valid: " << valid(blag) << endl;
  cout << "All together now: " << validInference(testString) << endl;
  cout << "Done\n";
  cout << ":DDDDDDDDDD\n";
  
  return 0;
} 