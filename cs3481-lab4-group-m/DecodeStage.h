//class to perform the combinational logic of
//the Fetch stage
class DecodeStage: public Stage
{
   private:
      void setEInput(E * ereg, uint64_t stat, uint64_t icode, uint64_t ifun, 
                     uint64_t valC, uint64_t valA, uint64_t valB, 
                     uint64_t valP, uint64_t dstE, uint64_t dstM,
                     uint64_t srcA, uint64_t srcB);
      uint64_t controlsrcA(uint64_t d_icode, D * dreg);
      uint64_t controlsrcB(uint64_t D_icode, D * dreg);
      uint64_t controldstE(uint64_t D_icode, D * dreg);
      uint64_t controldstM(uint64_t D_icode, D * dreg);
      uint64_t controlselFwdA(uint64_t D_icode, D * dreg);
      uint64_t controlFwdB(uint64_t D_icode, D * dreg);
      
   public:
      bool doClockLow(PipeReg ** pregs, Stage ** stages);
      void doClockHigh(PipeReg ** pregs);

};