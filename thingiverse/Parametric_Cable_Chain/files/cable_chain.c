#include "model.h"

#define inches_to_mm 25.4
#define degrees_to_rad(a) ((a/360.0)*PI2)


#define pin_to_pin_length (2.0*inches_to_mm)
#define maximal_width (2.0*inches_to_mm)
#define maximal_height (0.9*inches_to_mm)
#define pin_diameter 10.0
#define wall_thickness 3.0

#define slop 1.0

#define max_bend_angle degrees_to_rad(180.0/8.0)
#define pin_scale 0.97
#define inside_corner_radius 4.0

model()
{	
OBJECT cap;
OBJECT body;
	
	union();
		cone(maximal_height/2.0,maximal_height/2.0,maximal_width);
		rotate(PI/2.0,0,0);
		translate(pin_to_pin_length/2.0,0,maximal_width/2.0);
	
		cone(maximal_height/2.0,maximal_height/2.0,maximal_width-wall_thickness*2.0);
		rotate(PI/2.0,0,0);
		translate(-pin_to_pin_length/2.0,0,(maximal_width-wall_thickness*2.0)/2.0);
	
		box();
		translate(0.5,0,0);
		scale(pin_to_pin_length/2.0,maximal_height,maximal_width);
		box();
		translate(-0.5,0,0);
		scale(pin_to_pin_length/2.0,maximal_height,maximal_width-wall_thickness*2.0);
	
		cone(pin_diameter/2.0,pin_diameter/2.0,maximal_width*pin_scale); //make it slightly smaller for easier fit
		rotate(PI/2.0,0,0);
		translate(-pin_to_pin_length/2.0,0,maximal_width*pin_scale/2.0);
		
		//put edge on top left to act as a stop
		box();
		translate(-0.5,0.5,0);
		scale(maximal_height/2.0,maximal_height/2.0,maximal_width-wall_thickness*2.0);
		translate(-pin_to_pin_length/2.0,0,0);
		//put edge on bottom left to act as a stop
		box();
		translate(-0.5,-0.5,0);
		scale(maximal_height/2.0,maximal_height/2.0,maximal_width-wall_thickness*2.0);
		translate(-pin_to_pin_length/2.0,0,0);
	
	end();
	
	OBJECT midform = cut();
	
	difference();
		paste(midform);
		for(int i=0;i<64;i++)
		{					
		paste(midform);
		scale(1,1,1+slop/maximal_width); //for slop in hinge
			
		//position to rotate around post
		translate(pin_to_pin_length/2.0,0,0);		
		rotate(0,0, -(max_bend_angle*((double)i)/64.0) ); //only bend on one side
		//move rotated object into hole position
		
		translate(pin_to_pin_length/2.0-slop/2.0,0,0);
		}
		
		//cut out core		
		box();
		//scale((pin_to_pin_length+maximal_height*2.0)*1.1,maximal_height-wall_thickness*2.0, maximal_width-wall_thickness*4.0);
		scale((pin_to_pin_length+maximal_height*2.0)*1.1,maximal_height-(wall_thickness*2.0+inside_corner_radius), maximal_width-wall_thickness*4.0);
		translate(0,inside_corner_radius/2.0,0);
		box();
		translate(0,0.5,0);
		scale((pin_to_pin_length+maximal_height*2.0)*1.1,inside_corner_radius, maximal_width-(wall_thickness*4.0+inside_corner_radius*2.0));
		translate(0,-maximal_height/2.0+wall_thickness,0);
		
		
		
		cone(inside_corner_radius,inside_corner_radius,1.0);
		translate(0,-0.5,0);
		rotate(0,0,PI/2.0);
		scale((pin_to_pin_length+maximal_height*2.0)*1.1,1,1);
		translate(0,-(maximal_height-(wall_thickness*2.0+inside_corner_radius*2.0))/2.0, (maximal_width-(wall_thickness*4.0+inside_corner_radius*2.0))/2.0);
		
		
		cone(inside_corner_radius,inside_corner_radius,1.0);
		translate(0,-0.5,0);
		rotate(0,0,PI/2.0);
		scale((pin_to_pin_length+maximal_height*2.0)*1.1,1,1);
		translate(0,-(maximal_height-(wall_thickness*2.0+inside_corner_radius*2.0))/2.0, -(maximal_width-(wall_thickness*4.0+inside_corner_radius*2.0))/2.0);
		
		//cut out sloppy hole
		cone(pin_diameter/2.0+slop/2.0,pin_diameter/2.0+slop/2.0,maximal_width);
		rotate(PI/2.0,0,0);
		translate(pin_to_pin_length/2.0,0,maximal_width/2.0);
		
	end();
		
		
	midform=cut();
	
	// cut anything that comes through bottom during bend
	difference();
		paste(midform);
		for(int i=0;i<128;i++)
		{		
			box();			
			scale(pin_to_pin_length+maximal_height*2.0,maximal_height,maximal_width);
			translate(0,-maximal_height,0);
			rotate(0,0, (max_bend_angle*((double)i)/128.0) );
			translate(-pin_to_pin_length/2.0,0,0);				
		}
	end();
		
	midform=cut();	
		
	//cut out top
	difference();
		paste(midform);
		box();
		translate(0,0.5,0);
		scale((pin_to_pin_length+maximal_height*2.0)*1.1,wall_thickness*2.0, maximal_width-wall_thickness*4.0);		
		translate(0,maximal_height/2.0-wall_thickness*1.1,0);
		
		//cut out divets
		for(double i=0;i<1.0;i+=1.0/8.0)
		{
			sphere(wall_thickness/2.5);
			translate(i*(pin_to_pin_length+maximal_height)-(pin_to_pin_length+maximal_height)/2.0+wall_thickness,
				maximal_height/2.0-wall_thickness/2.0,
				maximal_width/2.0-wall_thickness*2.0);
			
			sphere(wall_thickness/2.5);
			translate(i*(pin_to_pin_length+maximal_height)-(pin_to_pin_length+maximal_height)/2.0+wall_thickness,
				maximal_height/2.0-wall_thickness/2.0,
				maximal_width/2.0-wall_thickness*2.0);
			scale(1,1,-1);
		}
	end();
		
	body=cut();
		
		
	//make cap	
	intersection();
		paste(midform);
		
		union();
			box();
			translate(0,0.5,0);
			scale((pin_to_pin_length+maximal_height*2.0)*1.1,wall_thickness*2.0, maximal_width-wall_thickness*4.0);		
			translate(0,maximal_height/2.0-wall_thickness*1.1,0);
			
			//divets
			for(double i=0;i<1.0;i+=1.0/8.0)
			{
				sphere(wall_thickness/2.5);
				scale(0.8,0.8,1);
				translate(i*(pin_to_pin_length+maximal_height)-(pin_to_pin_length+maximal_height)/2.0+wall_thickness,
					maximal_height/2.0-wall_thickness/2.0,
					maximal_width/2.0-wall_thickness*2.0);
				
				sphere(wall_thickness/2.5);
				scale(0.8,0.8,1);
				translate(i*(pin_to_pin_length+maximal_height)-(pin_to_pin_length+maximal_height)/2.0+wall_thickness,
					maximal_height/2.0-wall_thickness/2.0,
					maximal_width/2.0-wall_thickness*2.0);
				scale(1,1,-1);
			}
		end();
	end();
	
	scale(1,1,1.02); //slightly stretch it for more holding pressure
	
	cap=cut();
			
	//cut cap on top where it might restrict wire path
	difference();		
		paste(cap);
			
		box();
		scale(pin_to_pin_length+maximal_height*2.0, maximal_height-wall_thickness*2.0, maximal_width);		
		translate(-pin_to_pin_length/2.0,0,0);				
		rotate(0,0, max_bend_angle ); 		
			
		translate(-pin_to_pin_length/2.0,0,0);				
	end();
			
	cap=cut();
	
	
	
	world_z_up();
	
	paste(cap);
	save_stl("/tmp/top.stl", 0.3, 5);	
	cut();
	
	paste(body);	
	save_stl("/tmp/segment.stl", 0.3, 5);	
}
