#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclass;
#pragma link C++ class myobject+;
#pragma link C++ class vector<myobject>;
#pragma link C++ class vector<myobject>::iterator;
#pragma link C++ function operator==(const vector<myobject>::iterator&,const vector<myobject>::iterator&); 
#pragma link C++ function operator!=(const vector<myobject>::iterator&,const vector<myobject>::iterator&); 
#pragma link C++ function operator<=(const vector<myobject>::iterator&,const vector<myobject>::iterator&); 
#pragma link C++ function operator>=(const vector<myobject>::iterator&,const vector<myobject>::iterator&); 
#pragma link C++ function operator<(const vector<myobject>::iterator&,const vector<myobject>::iterator&); 
#pragma link C++ function operator>(const vector<myobject>::iterator&,const vector<myobject>::iterator&); 
#pragma link C++ function operator-(const vector<myobject>::iterator&,const vector<myobject>::iterator&); 
#pragma link C++ function operator+(const vector<myobject>::iterator::difference_type,const vector<myobject>::iterator&); 
#pragma link C++ class myGenobject+;
#pragma link C++ class vector<myGenobject>;
#pragma link C++ class vector<myGenobject>::iterator;
#pragma link C++ function operator==(const vector<myGenobject>::iterator&,const vector<myGenobject>::iterator&); 
#pragma link C++ function operator!=(const vector<myGenobject>::iterator&,const vector<myGenobject>::iterator&); 
#pragma link C++ function operator<=(const vector<myGenobject>::iterator&,const vector<myGenobject>::iterator&); 
#pragma link C++ function operator>=(const vector<myGenobject>::iterator&,const vector<myGenobject>::iterator&); 
#pragma link C++ function operator<(const vector<myGenobject>::iterator&,const vector<myGenobject>::iterator&); 
#pragma link C++ function operator>(const vector<myGenobject>::iterator&,const vector<myGenobject>::iterator&); 
#pragma link C++ function operator-(const vector<myGenobject>::iterator&,const vector<myGenobject>::iterator&); 
#pragma link C++ function operator+(const vector<myGenobject>::iterator::difference_type,const vector<myGenobject>::iterator&); 

#pragma link C++ class myevent+;
#endif


