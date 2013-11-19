																																
bearing_diameter = 		9 ;
bearing_width = 		4 ;
m4_diamter = 			4 ;
main_hole_diameter = 	5 ;
servo_horn_diameter = 	4.5 ;
horn_height = 			3.2 ;
foot_length = 			17  ;
foot_width = 			14.2 ;
layer_height = 			0.3 * 2 ;


$fn =					100 ;
rotate([0,0,180]){
color([0.5,0.5,0.5])
foot();
//mock_bearing();
support();
}
 
e_step_correct = 1.02;
true_filament_width = e_step_correct*.6;
//true_filament_width = .6;

echo(true_filament_width);
hinge_diameter = main_hole_diameter+true_filament_width*4;
servo_outer_diameter = servo_horn_diameter+true_filament_width*4;
 
module foot(){
translate([0,0,foot_width/2]){
	difference(){
		union(){
			hull(){
				translate ([2.0,0]) cylinder (r=m4_diamter/2+true_filament_width*2, h=foot_width, center = true);
				translate([foot_length, 0,0])
					cylinder (r=m4_diamter/2+true_filament_width*2, h=foot_width, center = true);
			} // end hull
			hull(){
				translate ([0,2,0]) 
					cylinder (r=servo_outer_diameter/2, h=  foot_width, center = true);
				translate ([2,0,0])
					cylinder (r=m4_diamter/2+true_filament_width*2, h=foot_width, center = true);
			} //  end hull
		} // end union
		
		translate ([0,2,0])cylinder(r=servo_horn_diameter/2, h=foot_width/2+.1);
		
		translate([0,2,-foot_width/2-.1])

		cylinder(r=servo_horn_diameter/2, h=foot_width/2+.1);
		
		translate([foot_length, 0,0])
			cylinder (r=m4_diamter/2, h=foot_width+.1, center = true);

		translate([foot_length, 0,0])
			cylinder (r=bearing_diameter/2+2, h=bearing_width*2+1.6, center = true);

	} // end difference
	//servo_mount();
	}
}

//servo_mount();

module servo_mount(){
	translate([0,2,foot_width/2-horn_height])
	{                            
		difference(){
		cylinder(r=servo_horn_diameter/2, h=1.2);
		cylinder(r=.5, h=1.3*5);
		}
		cylinder(r=servo_horn_diameter/2, h=.3);
	}        
}

module support(){
translate([0,0,foot_width/2])
	difference(){
		union(){
			hull(){
				cylinder (r=hinge_diameter/2+4, h=foot_width, center = true);
				translate([foot_length, 0,0])
					cylinder (r=bearing_diameter/2-1+4, h=foot_width, center = true);
			} // end hull
		} // end union
		translate([0,0,-foot_width/2+bearing_width+.8])
			hull(){
				cylinder (r=hinge_diameter/2+1, h=foot_width, 	center = true);
			translate([foot_length, 0,0])
				cylinder (r=bearing_diameter/2, h=foot_width, center = true);
			} // end hull
		
		translate([0,0,0])
		cube([18,40,40], center=true);
		translate([-40,-40,bearing_width+.8+layer_height])
		cube([80,80,30], center = false);
	
	} // end difference
			
}

module mock_bearing(){

translate([0,0,foot_width/2]){
	translate([foot_length, 0,-bearing_width-.8])
		{
		color ([1,1,1])
		cylinder (r=4.5 , h=.8);
		translate([0,0,.8])
		color([0.2,0.2,0.2])
		cylinder (r=bearing_diameter/2+1, h=bearing_width, center = false);
		translate([0,0,bearing_width+.8])
		color ([0.2,0.2,0.2])
		cylinder (r=bearing_diameter/2+1, h=bearing_width, center = false);
		translate([0,0,bearing_width*2+.8])
		color ([1,1,1])
		cylinder (r=4.5 , h=.8);
		}
		translate([foot_length, 0,0])
				color ([1,1,1])
		cylinder (r=m4_diamter/2, h=foot_width+.1, center = true);
	}
	}