
#ifndef GOGAME_INC
#define GOGAME_INC
 
#include "pligo/engine.gen/board/position.h"
#include "../gapic/gapicgc.h"

extern gapic_type Gapic_Gogame_Position_Type;

class GogamePositionCollectable :
public GapicCollectable<Position,&Gapic_Gogame_Position_Type> {
 private:
  BlockName blockid2name(int id) const {
    return BlockName(id&0xff,id>>8);}
 public:
  int boardpred(int x,int y) const {
    BlockName b=Get()->GetBlockNameXY(x,y);
    return (b.gety()<<8)+b.getx();}
  int valid_blockid(int blockid) const;
  int blocksizepred(int blockid) const {
     return Get()->GetBlock(blockid2name(blockid))->GetStones().size();}
  int blockcolorpred(int blockid) const {
    return Get()->GetBlock(blockid2name(blockid))->GetColor();}
  int libertycountpred(int blockid) const {
    return Get()->GetBlock(blockid2name(blockid))->GetLibertyCount();}
  int opponentcountpred(int blockid) const {
    return Get()->GetBlock(blockid2name(blockid))->GetOpponentCount();}
  int adjacentcountpred(int blockid) const {
    return Get()->GetBlock(blockid2name(blockid))->GetAdjacentCount();}
  Coordinate getstonepred(int blockid,int i) const {
    return Get()->GetBlock(blockid2name(blockid))->GetStones()[i];}
  BlockName getlibertypred(int blockid,int i) const {
    return *((Get()->GetBlock(blockid2name(blockid))->GetNeighbours().
	    begin_liberty())+i);}
  BlockName getopponentpred(int blockid,int i) const {
    return *((Get()->GetBlock(blockid2name(blockid))->GetNeighbours().
	    begin_opponent())+i);}
  BlockName getadjacentpred(int blockid,int i) const {
    return *((Get()->GetBlock(blockid2name(blockid))->GetNeighbours().
	    begin())+i);}
  void Print() const {cout << *(Get());}
  GogamePositionCollectable(Position *pos) :
    GapicCollectable<Position,&Gapic_Gogame_Position_Type>(pos) {};
};

#endif
 
