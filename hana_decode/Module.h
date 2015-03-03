#ifndef Module_
#define Module_

/////////////////////////////////////////////////////////////////////
//
//   Module
//   Abstract interface for a module that sits in a slot of a crate.
//
/////////////////////////////////////////////////////////////////////

#include <vector>
#include <set>
#include <fstream>
#include "TNamed.h"
#include "Decoder.h"
#include "DecoderGlobals.h"

namespace Decoder {

class Module : public TNamed  {

public:

  struct ModuleType {
   public:
     ModuleType ( const char *c1, Int_t i1 ) : fClassName(c1), fMapNum(i1), fTClass(0) {}
     bool operator<( const ModuleType& rhs ) const { return fMapNum < rhs.fMapNum; }
     const char*      fClassName;
     Int_t            fMapNum;
     mutable TClass*  fTClass;
  };

  Module(Int_t crate, Int_t slot);

  typedef std::set<ModuleType> TypeSet_t;
  typedef TypeSet_t::iterator TypeIter_t;
  static TypeSet_t& fgModuleTypes();

  Module() { };  // for ROOT TClass & I/O

  virtual ~Module();

// inheriting classes need to implement one or more of these
  virtual Int_t GetData(Int_t) { return 0; };
  virtual Int_t GetData(Int_t, Int_t) { return 0; };
  virtual Int_t GetData(Int_t, Int_t, Int_t) { return 0; };

// Making this class Abstract does not work (problem with Dictionary)
  virtual Int_t Decode(const UInt_t *p) {return 0;}; // implement in derived class
// Loads slot data
  virtual Int_t LoadSlot(THaSlotData *sldat,  const UInt_t *evbuffer, const UInt_t *pstop );

  virtual Int_t GetNumChan() { return fNumChan; };

  virtual Int_t GetNumEvents() { return 0; };
  virtual Int_t GetMode() { return 0; };

  virtual void SetSlot(Int_t crate, Int_t slot, Int_t header=0, Int_t mask=0, Int_t modelnum=0)
  { fCrate=crate; fSlot=slot; fHeader=header; fHeaderMask=mask; fModelNum=modelnum;};

  virtual void Init();

  virtual void Clear(const Option_t *opt) { fWordsSeen = 0; };

  virtual Bool_t IsSlot(UInt_t rdata);

  virtual Int_t GetCrate() { return fCrate; };
  virtual Int_t GetSlot() { return fSlot; };

  virtual void SetDebugFile(std::ofstream *file) { if (file!=0) fDebugFile = file; };

  virtual void SetHeader(UInt_t header, UInt_t mask) {
    fHeader = header;
    fHeaderMask = mask;
  }

  Module& operator=(const Module &rhs);

  virtual void DoPrint();

protected:

  static TypeIter_t DoRegister( const ModuleType& registration_info );

  Int_t fCrate, fSlot;
  std::vector<Int_t> fData;  // Raw data
  UInt_t fHeader, fHeaderMask;
  Int_t fWordsExpect, fWordsSeen;
  Int_t fWdcntMask, fWdcntShift;
  Int_t fModelNum, fNumChan;
  Bool_t IsInit;

  static TypeIter_t fgThisType;

  std::ofstream *fDebugFile;

  Module(const Module& rhs);
  void Create(const Module& rhs);


private:

  ClassDef(Module,0)  // A module in a crate and slot

};

}

#endif
