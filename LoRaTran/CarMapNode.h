#ifndef CARMAPNODE_H
#define CARMAPNODE_H


class CarMapNode
{
    public:
        CarMapNode(){}

        unsigned int GetGvalue() { return Gvalue; }
        void SetGvalue(unsigned int val) {
            Gvalue = val;
            Fvalue = Gvalue + Hvalue;
        }
        unsigned int GetHvalue() { return Hvalue; }
        void SetHvalue(unsigned int val) {
            Hvalue = val;
            Fvalue = Gvalue + Hvalue;
        }
        unsigned int GetFvalue() const { return Fvalue; }
        bool GetisWlakable() { return isWlakable; }
        void SetisWlakable(bool val) { isWlakable = val; }
        CarMapNode* GetfatherNode() { return fatherNode; }
        void SetfatherNode(CarMapNode* val) { fatherNode = val; }

        void SetCoordinate(int x,int y){
            cor_x = x;  cor_y = y;
        }
        int GetCor_x(){return cor_x;}
        int GetCor_y(){return cor_y;}

        void SetLon(double gps_lon){ lon = gps_lon; }
        void SetLat(double gps_lat){ lat = gps_lat; }

        double GetLon(){ return lon; }
        double GetLat(){ return lat; }

        void SetStronghold(int s){ stronghold = s; }
        int GetStronghold(){ return stronghold; }

        void SetstrongholdMark(bool mark) { strongholdMark = mark; }
        bool GetstrongholdMark() { return strongholdMark; }


        void setDir(int d){ dir = d; }
        int getDir(){ return dir; } 

    protected:

    private:
        int cor_x;
        int cor_y;
        unsigned int Gvalue = 0;
        unsigned int Hvalue = 0;
        unsigned int Fvalue = 0;
        bool isWlakable = true;
        CarMapNode* fatherNode;
        double lon = 0;
        double lat = 0;

        int stronghold = -1;
        bool strongholdMark = false;
        int dir = 0;
};

#endif // CARMAPNODE_H
