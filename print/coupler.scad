// 10.35 out for 1/4 threaded rod plus rubber tube
// nema 23 motor shaft is 6.3 mm diam


	bdiam = 4.2;
	bheight = 31;

module coupler()
{
	height = 30;
	diam = 32;// no more than ~36
	rodplus = 10;
	nema23 = 5;

	boltoffset = diam/3.5;

	difference()
	{
		// the main cylinder body
		cylinder(h= height,r = diam/2,$fn=77);
		translate(v = [0,0,-1])
			cylinder(h= height/2 +1,r = rodplus/2,$fn=77);
	
		translate(v = [0,0,height/2])
			cylinder(h= height/2 +1,r = nema23/2,$fn=77);

	translate(v=[boltoffset,bheight/2,height*.75])
		rotate(a=[90,0,0])
			bolt();

	translate(v=[boltoffset,bheight/2,height*.25])
		rotate(a=[90,0,0])
			bolt();

	translate(v=[-boltoffset,bheight/2,height*.75])
		rotate(a=[90,0,0])
			bolt();

	translate(v=[-boltoffset,bheight/2,height*.25])
		rotate(a=[90,0,0])
			bolt();
	
	translate(v=[0,0,height/2])
		cube(size = [diam+1,2,height+2],center = true);
	}
}

coupler();
/*
a machine bolt of diameter ~3.25
and length of 28mm
head is approx 6.4 mm
*/
module bolt()
{
	cylinder(h= bheight +1,r = bdiam/2,$fn=77);
	translate(v=[0,0,bheight - 4])
		cylinder(h= 5,r = 6.5/1.6,$fn=77);

	translate(v=[0,0,0])
		cylinder(h= 7,r = 9/2.05,$fn=6);

}
//bolt();

