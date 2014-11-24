#include "..\..\Engine\Engine\Advanced2D.h""
using namespace Advanced2D;

#include "Animator.h"

namespace Advanced2D
{
	//////////
	Animator_Bone::Animator_Bone()
	{
		this->button_bone = NULL;
		this->sprite_bone = NULL;		
	}
	/////////

	Animator::Animator()
	{
		this->sprite = NULL;
		this->animator_timer = NULL;
		this->frame_add = NULL;
		this->animator_form = NULL;
		this->play_animation = false;
		this->totalframes = 0;
		this->currentframe = 0;
		this->frame_tick = 0;
		this->scale_bone = 2;

	}

	void Animator::Reset()
	{
		//reset cursor
		g_engine->gui->setSelectedButton(NULL);
		g_engine->gui->setSelectedSprite(NULL);

		//reset variables
		this->play_animation=false;
		this->totalframes=0;
		this->currentframe=0;
		this->frame_tick=0;

		//delete sprite and his skeleton
		if (this->sprite)
		{
			this->sprite->setAlive(false);
			for(int i=0;i<this->sprite->skeleton.size();i++)
			{
				this->sprite->skeleton[i]->setAlive(false);
			}			
			this->sprite=NULL;
		}

		//delete animator form
		if(this->animator_form)
		{
			g_engine->gui->form_delete(this->animator_form);
			this->animator_form=NULL;
		}

		//delete buttons
		if (this->animator_timer)
		{
			this->animator_timer->setAlive(false);
			this->animator_timer = NULL;
		}

		if (this->frame_add)
		{
			this->frame_add->setAlive(false);
			this->frame_add = NULL;
		}		

		for(int i=0;i<this->frame_button.size();i++)
		{
			this->frame_button[i]->setAlive(false);
		}
		this->frame_button.clear();

		for(int i=0;i<this->bone_vector.size();i++)
		{
			this->bone_vector[i]->button_bone->setAlive(false);
			this->bone_vector[i]->sprite_bone->setAlive(false);
			for(int j=0;j<this->bone_vector[i]->frame_rotation.size();j++)
			{
				this->bone_vector[i]->frame_rotation[j]->setAlive(false);
			}			
		}
		this->bone_vector.clear();		

	}//Reset()

	bool Animator::LoadTextures()
	{
		if(!this->Animation_ListLoad()) return false;
		if(!this->Animation_ListSave()) return false;		

		return true;
	}

	void Animator::addSkeletonButtons()
	{		
		Button *temp_form = g_engine->gui->form_add("SELECT SKELETON",g_engine->getScreenWidth()/2-200,0,600,100,'L',true);

		int pos_x = temp_form->form[1]->getX() + 5;
		int pos_y = temp_form->form[1]->getY() + 5;
		int d = 2;

		//add skeleton buttons
		std::vector <Skeleton*>::iterator iter = g_engine->skeleton_manager.skeletons.begin();
		while(iter != g_engine->skeleton_manager.skeletons.end())
		{
			Skeleton *a = *iter;

			Button *skeleton_button = new Button();
			skeleton_button->setType(BUTTON_ANIMATOR_SKELETON);
			skeleton_button->setState(1);
			skeleton_button->setText(a->file_path);
			skeleton_button->setImage(g_engine->gui->default_t);		
			skeleton_button->setPosition(pos_x,pos_y);
			skeleton_button->setSize(500,g_engine->e_font->size+4);
			skeleton_button->setColor_Texture(0xFF666666);
			g_engine->gui->addButton(skeleton_button);

			temp_form->children.push_back(skeleton_button);
			skeleton_button->parent = temp_form;

			iter++;
			pos_y += skeleton_button->getHeight() + d;
		}

		g_engine->gui->form_fitpanel_w(temp_form);
		g_engine->gui->form_fitpanel_h(temp_form);

	}

	void Animator::setAnimator()
	{
		int d = 4;	
		int posx = 3;
		int posy = 4;

		int width = 100;
		int height = g_engine->e_font->size + 6;

		//TOOLBARS
		Button *menu_bar = new Button();
		menu_bar->setImage(g_engine->gui->default_t);		
		menu_bar->setPosition(0,0);
		menu_bar->setSize(g_engine->getScreenWidth(),height+8);
		menu_bar->clickable = false;
		menu_bar->setColor_Texture(0xFF666666);
		g_engine->gui->addButton(menu_bar);

		Button *play = new Button();
		play->setType(BUTTON_ANIMATOR_PLAY);
		play->setImage(g_engine->gui->default_t);
		play->setText("PLAY");
		play->setSize(width,height);
		play->setColor_Texture(0xFFCC0000);
		play->setPosition(posx,posy);
		g_engine->gui->addButton(play);

		posx += play->getWidth() + d;

		Button *double_b = new Button();
		double_b->setType(BUTTON_ANIMATOR_DOUBLE);
		double_b->setImage(g_engine->gui->default_t);
		double_b->setText("2X");
		double_b->setSize(width,height);
		double_b->setColor_Texture(0xFFCC0000);
		double_b->setPosition(posx,posy);
		g_engine->gui->addButton(double_b);

		posx += double_b->getWidth() + d;

		Button *new_b = new Button();
		new_b->setType(BUTTON_ANIMATOR_NEW);
		new_b->setImage(g_engine->gui->default_t);
		new_b->setText("NEW");
		new_b->setSize(width,height);
		new_b->setColor_Texture(0xFFCC0000);
		new_b->setPosition(posx,posy);
		g_engine->gui->addButton(new_b);

		posx += new_b->getWidth() + d;

		Button *reset = new Button();
		reset->setType(BUTTON_ANIMATOR_RESET);
		reset->setImage(g_engine->gui->default_t);
		reset->setText("RESET");
		reset->setSize(width,height);
		reset->setColor_Texture(0xFFCC0000);
		reset->setPosition(posx,posy);
		g_engine->gui->addButton(reset);

		posx += reset->getWidth() + d;

		Button *load = new Button();
		load->setType(BUTTON_ANIMATOR_LOAD);
		load->setImage(g_engine->gui->default_t);
		load->setText("ANIMATIONS");
		load->setState(0);
		load->setSize(width,height);
		load->setColor_Texture(0xFFCC0000);
		load->setPosition(posx,posy);
		g_engine->gui->addButton(load);

		posx += load->getWidth() + d;

		//SKELETONS
		Button *skeletons_b = new Button();
		skeletons_b->setType(BUTTON_ANIMATOR_SKELETON);
		skeletons_b->setImage(g_engine->gui->default_t);
		skeletons_b->setText("SKELETONS");
		skeletons_b->setState(0);
		skeletons_b->setSize(width,height);
		skeletons_b->setColor_Texture(0xFFCC0000);
		skeletons_b->setPosition(posx,posy);
		g_engine->gui->addButton(skeletons_b);

		posx += skeletons_b->getWidth() + d;		

		Button *save = new Button();
		save->setType(BUTTON_ANIMATOR_SAVE);
		save->setImage(g_engine->gui->default_t);
		save->setText("SAVE");
		save->setSize(width,height);
		save->setColor_Texture(0xFFCC0000);
		save->setPosition(posx,posy);
		g_engine->gui->addButton(save);

		posx += save->getWidth() + d;

		Button *save_as = new Button();
		save_as->setType(BUTTON_ANIMATOR_SAVEAS);
		save_as->setImage(g_engine->gui->default_t);
		save_as->setText("SAVE AS");
		save_as->setSize(width,height);
		save_as->setColor_Texture(0xFFCC0000);
		save_as->setPosition(posx,posy);
		g_engine->gui->addButton(save_as);

		posx += save_as->getWidth() + d;		

		//EXIT ANIMATOR
		Button *exit = new Button();
		exit->setType(BUTTON_ANIMATOR_EXIT);
		exit->setImage(g_engine->gui->default_t);
		exit->setText("EXIT");
		exit->setSize(width,height);
		exit->setColor_Texture(0xFFCC0000);
		exit->setPosition(g_engine->getScreenWidth()-exit->getWidth()-2,posy);
		g_engine->gui->addButton(exit);		

	}

	void Animator::setAnimationForm()
	{
		std::stringstream stream;

		this->animator_form = g_engine->gui->form_add(this->current_animation,0,40,120,g_engine->getScreenHeight(),'L',false);	
		this->animator_form->form[0]->clickable=false;
		this->animator_form->form[0]->setCollidable(false);

		int pos_x = this->animator_form->form[1]->getX() + 2*2;
		int pos_y = this->animator_form->form[1]->getY() + 2*2;

		Button *frame_timer = new Button();		
		frame_timer->setType(ANIMATOR_TIMER);
		frame_timer->setClass(TEXTBOX);
		frame_timer->setImage(g_engine->gui->default_t);
		frame_timer->setText("0");
		frame_timer->setSize(25,g_engine->e_font->size);		
		frame_timer->setPosition(pos_x,pos_y);		
		frame_timer->setColor_Texture(0xFFFFFFFF);
		frame_timer->setColor_Box(0xFF888888);
		frame_timer->setColor_Text(0xFF000000);	
		g_engine->gui->addButton(frame_timer);
		this->animator_timer=frame_timer;

		pos_x += frame_timer->getWidth() + 2;

		Button *add_frame = new Button();		
		add_frame->setImage(g_engine->gui->default_t);	
		add_frame->setText("+");
		add_frame->setSize(25,g_engine->e_font->size);
		add_frame->setType(BONE_ADDFRAME);
		add_frame->setPosition(pos_x,pos_y);
		add_frame->setColor_Texture(0xFF888888);
		add_frame->setColor_Text(0xFF000000);
		g_engine->gui->addButton(add_frame);		
		this->frame_add=add_frame;

		pos_y += add_frame->getHeight() + 2*2;
		pos_x = this->animator_form->form[1]->getX() + 2*2 + 2*2;

		//frame button
		Button *frame = new Button();
		frame->setImage(g_engine->gui->default_t);		
		frame->setType(BONE_FRAME);		
		frame->setID(0);
		frame->setText("0");
		frame->setSize(25,g_engine->e_font->size);
		frame->setPosition(pos_x,pos_y);
		frame->setColor_Texture(0xFF888888);
		frame->setColor_Text(0xFF000000);
		g_engine->gui->addButton(frame);
		this->frame_button.push_back(frame);

		this->totalframes++;

		this->animator_form->children.push_back(this->frame_add);
		this->animator_form->children.push_back(this->animator_timer);
		this->animator_form->children.push_back(frame);
		g_engine->gui->form_fitpanel_w(this->animator_form);
		g_engine->gui->form_fitpanel_h(this->animator_form);
	}

	Button* Animator::addBone(Sprite *a, int x, int y)
	{
		Animator_Bone *bone = new Animator_Bone();
		this->bone_vector.push_back(bone);		

		//bone button
		Button *button_bone = new Button();
		button_bone->setImage(g_engine->gui->default_t);	
		button_bone->setText(a->getName());
		button_bone->setSize(75,g_engine->e_font->size*2+2);
		button_bone->setType(BUTTON_BONE);
		button_bone->setPosition(x,y);
		button_bone->setColor_Texture(0xFF888888);
		button_bone->setColor_Text(0xFF000000);
		g_engine->gui->addButton(button_bone);
		button_bone->child_sprite=a;

		//frame rotation
		Button *rotation = new Button();
		rotation->setImage(g_engine->gui->default_t);
		rotation->setText("0");
		rotation->setSize(25,g_engine->e_font->size);
		rotation->setClass(TEXTBOX);
		rotation->setPosition(x+2*2,y+g_engine->e_font->size);
		rotation->setColor_Texture(0xFFFFFFFF);
		rotation->setColor_Box(0xFF888888);
		rotation->setColor_Text(0xFF000000);
		g_engine->gui->addButton(rotation);

		bone->sprite_bone=a;
		bone->button_bone=button_bone;
		bone->frame_rotation.push_back(rotation);

		this->animator_form->children.push_back(button_bone);
		g_engine->gui->form_fitpanel_w(this->animator_form);
		g_engine->gui->form_fitpanel_h(this->animator_form);

		return button_bone;
	}

	void Animator::addFrameColumn()
	{
		this->totalframes++;

		std::stringstream stream;
		stream << frame_button.size()-1;

		stream.str("");
		stream << frame_button.size();

		//frame button
		Button *frame = new Button();
		frame->setImage(g_engine->gui->default_t);		
		frame->setType(BONE_FRAME);		
		frame->setID(frame_button.size());
		frame->setText(stream.str());
		frame->setSize(25,g_engine->e_font->size);
		frame->setPosition(
			this->frame_button[this->frame_button.size()-1]->getX()+
			this->frame_button[this->frame_button.size()-1]->getWidth()+2,
			this->frame_button[this->frame_button.size()-1]->getY()
			);
		frame->setColor_Texture(0xFF888888);
		frame->setColor_Text(0xFF000000);
		g_engine->gui->addButton(frame);

		this->frame_button.push_back(frame);

		//bone frames
		int pos_x=10;
		int pos_y=100;

		for(int i=0;i<this->bone_vector.size();i++)
		{
			//frame rotation
			Button *rotation = new Button();
			rotation->setImage(g_engine->gui->default_t);					
			rotation->setSize(25,g_engine->e_font->size);
			rotation->setClass(TEXTBOX);
			rotation->setText(bone_vector[i]->frame_rotation[bone_vector[i]->frame_rotation.size()-1]->getText());
			rotation->setColor_Texture(0xFFFFFFFF);
			rotation->setColor_Box(0xFF888888);
			rotation->setColor_Text(0xFF000000);
			g_engine->gui->addButton(rotation);

			rotation->setPosition(
				bone_vector[i]->frame_rotation[bone_vector[i]->frame_rotation.size()-1]->getX()+
				bone_vector[i]->frame_rotation[bone_vector[i]->frame_rotation.size()-1]->getWidth()+2,
				bone_vector[i]->frame_rotation[bone_vector[i]->frame_rotation.size()-1]->getY()
				);				

			bone_vector[i]->frame_rotation.push_back(rotation);

			this->bone_vector[i]->button_bone->setWidth(
				this->bone_vector[i]->button_bone->getWidth()+
				bone_vector[i]->frame_rotation[bone_vector[i]->frame_rotation.size()-1]->getWidth()+2
				);
		}

		g_engine->gui->form_fitpanel_w(this->animator_form);
		g_engine->gui->form_fitpanel_h(this->animator_form);
	}	

	void Animator::Update_sprite(int currentframe)
	{
		if(this->sprite)
		{
			this->sprite->setFrameCurrent(currentframe);
			g_engine->skeleton_manager.Walk_Bipedal(this->sprite);
		}		
	}

	void Animator::Update()
	{
		if (this->sprite)
		{
			g_engine->coll_manager->collbox_update(this->sprite);
		}

		if (this->play_animation)
		{
			std::stringstream temp;
			int value=0;

			if (this->animator_timer)
			{
				temp << this->animator_timer->getText();
				if (!(temp>>value)) value = 0;
			}

			if (this->frame_tick<value/14)
				this->frame_tick+=g_engine->game_speed*g_engine->GetDeltaFix();
			else
			{
				//update current frame
				this->currentframe++;
				if (this->currentframe > this->totalframes-1) this->currentframe=0;
				//

				//show which frame is displayed
				for(int i=0;i<this->frame_button.size();i++)
				{
					this->frame_button[i]->selected=false;
				}
				this->frame_button[this->currentframe]->selected=true;
				//

				//update skeleton
				for(int i=0;i<this->sprite->skeleton.size();i++)
				{
					double value;
					std::stringstream stream;
					stream << this->bone_vector[i]->frame_rotation[this->currentframe]->getText();
					if (stream >> value) this->sprite->skeleton[i]->setRotation(g_engine->math->toRadians(value));	
				}
				this->Update_sprite(this->currentframe);
				//

				//reset timer
				this->frame_tick=0;
			}
		}
	}

	void Animator::Draw()
	{
		g_engine->p_device->SetRenderTarget(0,g_engine->pBackBuffer);
		g_engine->ar_graphics->ClearScene(0xFF000000);

		if (g_engine->getRenderType()==TEXTURE || g_engine->getRenderType()==ALL)
		{
			for(int i=0;i<g_engine->game_sprites.size();i++)
				g_engine->game_sprites[i]->draw(TEXTURE);			
		}

		if (g_engine->getRenderType()==VECTOR || g_engine->getRenderType()==ALL)
		{
			for(int i=0;i<g_engine->game_sprites.size();i++)
				g_engine->game_sprites[i]->draw(VECTOR);
		}
		
		
		if(g_engine->gui->getSelectedSprite())
		{
			Sprite *sprite = g_engine->gui->getSelectedSprite();

			//bones
			for(int i=0;i<sprite->att_angle.size();i++)
			{
				g_engine->ar_graphics->draw_line(
					sprite->getX(),sprite->getY(),
					sprite->getAtt_posX(i),sprite->getAtt_posY(i),
					0,0,0xFF00FF00);
			}

			//joints
			g_engine->ar_graphics->draw_dot(sprite->getX(),sprite->getY(),0,0,0xFFFFFF00);

			//kada selektiram glavni sprite, a ne bone
			//edit bone
			if(sprite->getImage())
			{
				//sprite and box
				g_engine->ar_graphics->draw_texture(
					sprite->getImage(),
					g_engine->getScreenWidth()/2,g_engine->getScreenHeight()/2,
					sprite->getImage()->getWidth(),sprite->getImage()->getHeight(),
					0,0,
					sprite->getImage()->getWidth(),sprite->getImage()->getHeight(),
					this->scale_bone,this->scale_bone,
					0xFFFFFFFF);

				g_engine->ar_graphics->draw_box(
					g_engine->getScreenWidth()/2,g_engine->getScreenHeight()/2,
					sprite->getImage()->getWidth()*this->scale_bone,sprite->getImage()->getHeight()*this->scale_bone,
					0,0,0xFFFFFFFF);

				//roation axis
				g_engine->ar_graphics->draw_dot(
					g_engine->getScreenWidth()/2+(int)(sprite->getRotateX()*this->scale_bone),
					g_engine->getScreenHeight()/2+(int)(sprite->getRotateY()*this->scale_bone),
					0,0,0xFFFFFF00);

				double temp_rotation = sprite->getRotation();
				sprite->setRotation(0);

				//treba izvorne vrijednosti negdi pohranit, a ne se ovako zajbavat

				//zasto se dvi tocke preklapaju ???
				for(int i=0;i<sprite->att_angle.size();i++)
				{
					g_engine->ar_graphics->draw_line(
						g_engine->getScreenWidth()/2 + (int)(sprite->getRotateX()*this->scale_bone),
						g_engine->getScreenHeight()/2 + (int)(sprite->getRotateY()*this->scale_bone),
						g_engine->getScreenWidth()/2 + (int)sprite->att_posx[i]*this->scale_bone,
						g_engine->getScreenHeight()/2 + (int)sprite->att_posy[i]*this->scale_bone,
						0,0,0xFF00FF00);

					g_engine->ar_graphics->draw_dot(
						g_engine->getScreenWidth()/2 + (int)sprite->att_posx[i]*this->scale_bone,
						g_engine->getScreenHeight()/2 + (int)sprite->att_posy[i]*this->scale_bone,
						0,0,0xFFFF8800);

					g_engine->font_manager.fonts[0]->PrintText(
						g_engine->inttostring(i+1),
						g_engine->getScreenWidth()/2 + (int)sprite->att_posx[i]*this->scale_bone,
						g_engine->getScreenHeight()/2 + (int)sprite->att_posy[i]*this->scale_bone - g_engine->font_manager.fonts[0]->getHeight() - 2,
						1,1,0xFFFF8800);
				}

				sprite->setRotation(temp_rotation);
			}
		}		

		if(g_engine->p_input->GetKeyState(DIK_TAB) & 0x80)
		{
			if(this->sprite!=NULL)
			{
				g_engine->font_manager.fonts[0]->PrintText(
					g_engine->skeleton_manager.GetSkeleton(this->current_skeleton)->file_text,
					10,40,1,2,0xFFFFFFFF);				
			}
		}

	}

	void Animator::double_frames()//ne radi kada je puni krug (np 150 i -160), krivo izracuna
	{
		//copy data to vector
		std::vector <std::vector <int>> data(this->bone_vector.size(),std::vector <int> (this->frame_button.size()));

		for(int i=0;i<this->bone_vector.size();i++)
		{
			for(int j=0;j<this->frame_button.size();j++)
			{
				std::stringstream temp;
				int value;
				temp << this->bone_vector[i]->frame_rotation[j]->getText();
				if(temp >> value) data[i][j]= value;
				else data[i][j]=0;//bolje da prekine i izbaci poruku

				this->bone_vector[i]->frame_rotation[j]->setText("");//reset
			}
		}

		int num = this->frame_button.size();//mijenja se u petlji pa zato treba sacuvat vrijednost prije

		if(num>1)
		{
			for(int i=0;i<num;i++) this->addFrameColumn();
		}

		//enter original values every second frame
		for(int i=0;i<data.size();i++)
		{
			for(int j=0;j<data[i].size();j++)
			{
				std::stringstream temp;
				temp << data[i][j];
				this->bone_vector[i]->frame_rotation[j*2]->setText(temp.str());
			}
		}

		//enter new values
		for(int i=0;i<this->bone_vector.size();i++)
		{
			for(int j=0;j<this->frame_button.size();j++)
			{				
				if( this->bone_vector[i]->frame_rotation[j]->getText() == "" )
				{
					if(j==this->frame_button.size()-1)//last and first
					{
						int prev = atoi(this->bone_vector[i]->frame_rotation[j-1]->getText().c_str());
						int post = atoi(this->bone_vector[i]->frame_rotation[0]->getText().c_str());
						int middle = (prev+post)/2;

						std::stringstream temp;
						temp << middle;

						this->bone_vector[i]->frame_rotation[j]->setText(temp.str());
					}
					else//middle values
					{
						int prev = atoi(this->bone_vector[i]->frame_rotation[j-1]->getText().c_str());
						int post = atoi(this->bone_vector[i]->frame_rotation[j+1]->getText().c_str());
						int middle = (prev+post)/2;

						std::stringstream temp;
						temp << middle;

						this->bone_vector[i]->frame_rotation[j]->setText(temp.str());
					}						
				}				
			}
		}//new values


		std::stringstream stream;
		int new_timer;

		stream << this->animator_timer->getText();
		stream >> new_timer;
		stream.str("");

		std::stringstream stream2;
		stream2 << new_timer/2;
		this->animator_timer->setText(stream2.str());		

	}

	//////////////////////
	//     CONTROLS     //
	//////////////////////

	void Animator::keyPress(int key)
	{
		if(g_engine->gui->getSelectedSprite()==NULL) return;
		Sprite *a = g_engine->gui->getSelectedSprite();

		switch (key)
		{
		case DIK_R:
			a->setRotation(
				g_engine->math->AngleToTarget(					
				a->getX(),a->getY(),
				g_engine->gui->cursor->getX(),g_engine->gui->cursor->getY()));
			this->Update_sprite(currentframe);
			for(unsigned i=0;i<this->bone_vector.size();i++)
			{
				if(a==bone_vector[i]->sprite_bone)
				{
					//update rotation textbox
					std::stringstream stream;
					stream << (int)g_engine->math->toDegrees(a->getRotation());
					bone_vector[i]->frame_rotation[this->currentframe]->setText(stream.str());
				}
			}
			break;
		}

	}

	void Animator::keyRelease(int key)
	{
		if(key==DIK_F5) this->SaveSkeleton();

		if(g_engine->gui->getSelectedSprite())
		{
			if(key==DIK_RETURN)
			{
				g_engine->gui->getSelectedSprite()->setRotationAxis(
					(g_engine->gui->cursor->getX()-g_engine->getScreenWidth()/2)/this->scale_bone,
					(g_engine->gui->cursor->getY()-g_engine->getScreenHeight()/2)/this->scale_bone
					);
			}

			if (g_engine->p_input->GetKeyState(DIK_LSHIFT)&0x80)
			{
				if(key==DIK_1)
				{
					g_engine->gui->getSelectedSprite()->setAtt_pos(0,
						(g_engine->gui->cursor->getX()-g_engine->getScreenWidth()/2)/this->scale_bone,
						(g_engine->gui->cursor->getY()-g_engine->getScreenHeight()/2)/this->scale_bone);
					return;
				}

				if(key==DIK_2)
				{
					g_engine->gui->getSelectedSprite()->setAtt_pos(1,
						(g_engine->gui->cursor->getX()-g_engine->getScreenWidth()/2)/this->scale_bone,
						(g_engine->gui->cursor->getY()-g_engine->getScreenHeight()/2)/this->scale_bone);
					return;
				}

				if(key==DIK_3)
				{
					g_engine->gui->getSelectedSprite()->setAtt_pos(2,
						(g_engine->gui->cursor->getX()-g_engine->getScreenWidth()/2)/this->scale_bone,
						(g_engine->gui->cursor->getY()-g_engine->getScreenHeight()/2)/this->scale_bone);
					return;
				}

				if(key==DIK_4)
				{
					g_engine->gui->getSelectedSprite()->setAtt_pos(3,
						(g_engine->gui->cursor->getX()-g_engine->getScreenWidth()/2)/this->scale_bone,
						(g_engine->gui->cursor->getY()-g_engine->getScreenHeight()/2)/this->scale_bone);
					return;
				}

				if(key==DIK_5)
				{
					g_engine->gui->getSelectedSprite()->setAtt_pos(4,
						(g_engine->gui->cursor->getX()-g_engine->getScreenWidth()/2)/this->scale_bone,
						(g_engine->gui->cursor->getY()-g_engine->getScreenHeight()/2)/this->scale_bone);
					return;
				}

				if(key==DIK_6)
				{
					g_engine->gui->getSelectedSprite()->setAtt_pos(5,
						(g_engine->gui->cursor->getX()-g_engine->getScreenWidth()/2)/this->scale_bone,
						(g_engine->gui->cursor->getY()-g_engine->getScreenHeight()/2)/this->scale_bone);
					return;
				}
			}
		}

	}

	void Animator::mousePress(int button){}

	void Animator::mouseRelease(int button)
	{
		switch(button)
		{
		case 0:
			g_engine->gui->setSelectedSprite(NULL);
			break;

		case 1:
			if(g_engine->gui->getSelectedSprite()==NULL) g_engine->gui->CursorCollision();			
			break;
		}
	}

	void Animator::mouseWheel(int wheel)
	{
		//treba nekako sacuvat originalni scale ako mislim zumirat i onda sacuvat u file
		
		if(wheel>0)
			g_engine->skeleton_manager.Scale(this->sprite->skeleton,1.2,1.2);
		else
			g_engine->skeleton_manager.Scale(this->sprite->skeleton,0.8,0.8);
			
	}

	void Animator::buttonPress(Button *button, int mousebutton){}

	void Animator::buttonRelease(Button *button, int mousebutton)
	{
		switch(mousebutton)
		{
		case 0:
			switch(button->getType())
			{
				//ANIMATOR CONTROLS
			case BUTTON_ANIMATOR_PLAY:
				if(this->sprite && !this->bone_vector.empty())
				{
					this->play_animation=!this->play_animation;
					if (this->play_animation)
					{
						for(int i=0;i<this->frame_button.size();i++)
						{
							this->frame_button[i]->selected=false;
						}
						this->frame_button[0]->selected=true;				
						this->currentframe=0;
					}
					this->frame_tick=0;
				}
				if(button->getText()=="PLAY")
					button->setText("STOP");
				else
					button->setText("PLAY");
				break;

			case BUTTON_ANIMATOR_NEW:this->Animation_new();break;

			case BUTTON_ANIMATOR_SKELETON:				
				if(button->getState()==0)
					this->addSkeletonButtons();
				else
				{
					if(this->sprite==NULL)
					{
						this->sprite = new Sprite();
						this->sprite->setSize(200,200);
						this->sprite->setPosition(
							g_engine->getScreenWidth()/2+g_engine->getScreenWidth()/4-this->sprite->w(),
							g_engine->getScreenHeight()/2-this->sprite->h());
						g_engine->coll_manager->setColl_box(this->sprite);
						g_engine->coll_manager->collbox_update(this->sprite);

						//treba napravit zastitu
						//set skeleton
						g_engine->skeleton_manager.GetSkeleton(button->getText())->setSkeleton(this->sprite);
						this->current_skeleton = button->getText();
						this->Update_sprite(currentframe);
						g_engine->addSprite(this->sprite,0);

					}	
					else
						g_engine->gui->messagebox_add("Error","Sprite already loaded !",10);

					g_engine->gui->form_delete(button->parent);
				}
				break;

			case BUTTON_ANIMATOR_LOAD:				
				if(button->getState()==0)
					this->addLoadButtons(BUTTON_ANIMATOR_LOAD);
				else
				{
					this->Animation_load(button->getText());
					g_engine->gui->form_delete(button->parent);
				}
				break;

			case BUTTON_ANIMATOR_SAVE:this->Animation_save(this->current_animation);break;
			case BUTTON_ANIMATOR_SAVEAS:this->addLoadButtons(BUTTON_ANIMATOR_SAVETOFILE);break;

			case BUTTON_ANIMATOR_SAVETOFILE:
				if (button->getClass()!=TEXTBOX)
				{
					if(button->getState()==0)
					{
						Button *temp_button = g_engine->gui->form_add_yesno(
							"SAVE ANIMATION","Overwrite existing animation ?",250,200);
						temp_button->form[3]->setType(BUTTON_ANIMATOR_SAVETOFILE);
						temp_button->form[3]->setText(button->getText());
						temp_button->form[3]->setWidth(temp_button->form[3]->getWidth()+16);
						temp_button->form[3]->setX(temp_button->form[3]->getX()-16);
						temp_button->form[3]->setState(1);
						temp_button->parent = button;
					}
					else
					{
						this->Animation_save(button->getText());
						//parent botuna je forma, parent forme je textbox,parent textboxa je forma
						if(button->parent->parent) g_engine->gui->form_delete(button->parent->parent->parent);
						g_engine->gui->form_delete(button->parent);			
					}
				}
				break;


			case BUTTON_ANIMATOR_DOUBLE:this->double_frames();break;
			case BUTTON_ANIMATOR_RESET:this->Reset();break;
			case BONE_ADDFRAME:this->addFrameColumn();break;
			
				//ANIMATION CONTROLS
			case BUTTON_BONE:
				button->selected = !button->selected;
				if(button->selected)
					g_engine->gui->setSelectedSprite((Sprite*)button->child_sprite);
				else
					g_engine->gui->setSelectedSprite(NULL);
				break;			

			case BONE_FRAME:	
				this->currentframe=button->getID();
				for(int i=0;i<this->sprite->skeleton.size();i++)
				{
					std::stringstream stream;
					double value;
					stream << this->bone_vector[i]->frame_rotation[button->getID()]->getText();
					if (stream >> value) this->sprite->skeleton[i]->setRotation(g_engine->math->toRadians(value));	
				}
				this->Update_sprite(currentframe);	
				for(int i=0;i<this->frame_button.size();i++)
				{
					this->frame_button[i]->selected=false;
				}
				button->selected=true;				
				break;			
			}
		}

	}//buttonRelease()

	//////////////////////////////////
	//     LOAD & SAVE ANIMATION    //
	//////////////////////////////////

	bool Animator::Animation_ListLoad()
	{
		//load list of animations
		this->animation_list.clear();		
		std::string filename = "Data/Animator/animation_list.txt";
		std::ifstream filein(filename.c_str());

		if(!filein.is_open())
		{
			g_engine->gui->messagebox_add("ERROR","File " + filename + " not found !",10);
			return false;
		}

		std::string line;
		while(std::getline(filein,line))
		{
			std::string temp_string = "Data/Animator/" + line + ".txt";
			std::ifstream temp_file(temp_string.c_str());

			if(temp_file.is_open())//test if file really exists
			{
				this->animation_list.push_back(line);
				temp_file.close();
			}
		}

		filein.close();		

		return true;

	}//Animation_ListLoad()

	bool Animator::Animation_ListSave()
	{
		//save refreshed animation list
		std::string filename = "Data/Animator/animation_list.txt";
		std::ofstream fileout(filename.c_str());
		if(!fileout.is_open())
		{
			g_engine->gui->messagebox_add("Error","File " + filename + " not created !",10);
			return false;
		}

		for(unsigned int i=0;i<this->animation_list.size();i++)
		{
			fileout << this->animation_list[i] << std::endl;
		}

		fileout.close();

		return true;

	}//Animation_ListSave()

	bool Animator::Animation_ListOn(std::string filename)
	{
		//check if already exists
		for(unsigned int i=0;i<this->animation_list.size();i++)
		{
			if(this->animation_list[i]==filename) return true;
		}

		return false;

	}

	void Animator::Animation_new()
	{
		//check if there is a sprite with skeleton loaded, and is Animator already loaded
		if(this->sprite==NULL)
		{
			g_engine->gui->messagebox_add("Error","No sprite found !",10);
			return;
		}
		else if(this->sprite->skeleton.empty())
		{
			g_engine->gui->messagebox_add("Error","No sprite with skeleton found !",10);
			return;
		}
		else if (!this->bone_vector.empty())
		{
			g_engine->gui->messagebox_add("Error","Animator already set !",10);
			return;
		}

		this->current_animation = "new_animation";
		this->setAnimationForm();

		//for each bone add a new set of buttons
		int pos_y = this->frame_add->getY()+this->frame_add->getHeight()*2+2*2*2;
		for(int i=0;i<this->sprite->skeleton.size();i++)
		{
			Button *temp_button = this->addBone(
				this->sprite->skeleton[i],
				this->animator_timer->getX(),pos_y);
			pos_y += temp_button->getHeight() + 2;						
		}

	}//Animation_new()

	void Animator::addLoadButtons(int button_type)
	{
		std::string form_title = "LOAD ANIMATION";
		if(button_type==BUTTON_ANIMATOR_SAVETOFILE) form_title = "SAVE ANIMATION";

		Button *temp_form = g_engine->gui->form_add(
			form_title,
			g_engine->getScreenWidth()/2-200,0,
			400,100,
			'L',true);

		int pos_x = temp_form->form[1]->getX() + 5;
		int pos_y = temp_form->form[1]->getY() + 5;
		int d = 2;

		if(button_type==BUTTON_ANIMATOR_SAVETOFILE)
		{
			Button *save_b = new Button();
			save_b->setClass(TEXTBOX);
			save_b->setType(BUTTON_ANIMATOR_SAVETOFILE);
			save_b->setState(0);//ask to overwrite
			save_b->setImage(g_engine->gui->default_t);
			save_b->setSize(300,g_engine->e_font->size);
			save_b->setColor_Texture(0xFFFFFFFF);
			save_b->setColor_Text(0xFF000000);
			save_b->setPosition(pos_x,pos_y);
			g_engine->gui->addButton(save_b);

			save_b->parent = temp_form;
			temp_form->form.push_back(save_b);

			pos_y += save_b->getHeight() + 2*2;		
		}		

		//ADD BUTTONS
		for(unsigned int i=0;i<this->animation_list.size();i++)
		{			
			Button *animation_b = new Button();
			animation_b->setType(button_type);
			animation_b->setImage(g_engine->gui->default_t);
			animation_b->setText(this->animation_list[i]);
			if(button_type==BUTTON_ANIMATOR_SAVETOFILE)
				animation_b->setState(0);
			else
				animation_b->setState(1);
			animation_b->setSize(300,g_engine->e_font->size);
			animation_b->setColor_Texture(temp_form->getColor_Texture());
			animation_b->setColor_Text(0xFFFFFFFF);
			animation_b->setPosition(pos_x,pos_y);
			g_engine->gui->addButton(animation_b);

			temp_form->children.push_back(animation_b);
			animation_b->parent=temp_form;

			pos_y += animation_b->getHeight() + d;

		}

		g_engine->gui->form_fitpanel_w(temp_form);
		g_engine->gui->form_fitpanel_h(temp_form);

	}//addLoadButtons()

	void Animator::Animation_load(std::string filename)
	{
		//check if there is a sprite with skeleton loaded, and is Animator already loaded
		if(this->sprite==NULL)
		{
			g_engine->gui->messagebox_add("Error","No sprite found !",10);
			return;
		}
		else if(this->sprite->skeleton.empty())
		{
			g_engine->gui->messagebox_add("Error","No sprite with skeleton found !",10);
			return;
		}
		else if (!this->bone_vector.empty())
		{
			g_engine->gui->messagebox_add("Error","Animator already set !",10);
			return;
		}

		//open the animation file
		std::string filename_fin = "Data/Animator/" + filename + ".txt";
		std::ifstream filein(filename_fin.c_str());
		if(!filein.is_open())
		{
			g_engine->gui->messagebox_add("Error","File \"" + filename_fin + "\" not found !",10);
			return;
		}

		this->current_animation = filename;

		std::string command;
		std::string line;
		double value=0;
		int frame_num=0;
		int bone_num=0;
		int timer=0;
		std::vector <std::string> file_vector;

		//read the data from file and store it
		while(std::getline(filein, line))
		{
			file_vector.push_back(line);

			std::stringstream stream(line);
			if (stream >> command >> value)
			{
				if (command=="bones")
				{
					bone_num=value;
				}
				else if (command=="frames")
				{
					frame_num=value;
				}
				else if (command=="timer")
				{
					timer=value;
				}
			}
		}
		filein.close();

		//check if number of bones are equal
		/*if(this->sprite->skeleton.size()!=bone_num)
		{
			std::stringstream stream;
			stream << "[" << this->sprite->skeleton.size() << "," << bone_num << "]";

			g_engine->gui->messagebox_add(
				"Error",
				"Skeleton and animation mismatch ! "+stream.str(),10);
			return;
		}*/

		//set animation form
		this->setAnimationForm();		

		std::stringstream temp;
		temp << timer ;
		this->animator_timer->setText(temp.str());

		//for each bone add a new set of buttons
		int pos_y = this->frame_add->getY()+this->frame_add->getHeight()*2+2*2*2;
		for(int i=0;i<this->sprite->skeleton.size();i++)
		{
			Button *temp_button = this->addBone(
				this->sprite->skeleton[i],
				this->animator_timer->getX(),pos_y);
			pos_y += temp_button->getHeight() + 2;						
		}

		//add required number of frame text boxes
		for(int i=0;i<frame_num-1;i++)
		{
			this->addFrameColumn();			
		}

		//set the angles in to the frame text boxes
		for(int i=0;i<this->bone_vector.size();i++)		
		{
			int k=0;
			while(k<file_vector.size())		
			{
				std::stringstream stream(file_vector[k]);
				std::string command2;
				if (stream >> command2)
				{
					//check if button name is equal to bone name
					if (command2==bone_vector[i]->sprite_bone->getName())
					{
						//check if there is data below						
						k++;
						if (k>file_vector.size()-1)
						{
							g_engine->gui->messagebox_add(
								"Error",
								"Missing animation data ["+bone_vector[i]->sprite_bone->getName()+"] !",10);
							return;
						}

						//read the data below
						std::stringstream stream2(file_vector[k]);
						for(int j=0;j<this->totalframes;j++)
						{
							std::stringstream temp;
							//check if data are numbers
							if (!(stream2 >> value))
							{
								g_engine->gui->messagebox_add(
									"Error",
									"Missing animation data ["+bone_vector[i]->sprite_bone->getName()+"] !",10);
								return;

							}
							temp << value;
							//put the data in apropriate frame text box
							bone_vector[i]->frame_rotation[j]->setText(temp.str());
						}
					}					
				}
				k++;
			}
		}	

		this->animator_form->setText(filename);

	}//Animation_load()

	bool Animator::Animation_save(std::string filename)
	{
		std::string filename_fin = "Data/Animator/" + filename + ".txt";
		std::ofstream fileout(filename_fin.c_str());
		if(!fileout.is_open()) return false;

		fileout << "bones " << bone_vector.size() << endl;
		fileout << "frames " << bone_vector[0]->frame_rotation.size() << endl;
		fileout << "timer " << this->animator_timer->getText() << endl;

		for(int i=0;i<this->bone_vector.size();i++)
		{
			fileout << bone_vector[i]->sprite_bone->getName() << endl;
			for(int j=0;j<bone_vector[i]->frame_rotation.size();j++)
			{
				fileout << bone_vector[i]->frame_rotation[j]->getText() << " ";
			}
			fileout << endl;
		}

		fileout.close();

		//add to animation list and refresh file
		if(!this->Animation_ListOn(filename)) this->animation_list.push_back(filename);
		this->Animation_ListSave();

		this->animator_form->setText(filename);

		return true;

	}//Animation_save()

	void Animator::SaveSkeleton()
	{
		//treba popravit ona zadnja dva broja za redosljed iscrtavanja

		if(!this->sprite) return;

		std::ofstream fileout("skeleton.txt");

		Sprite *a = this->sprite;

		for(unsigned int i=0;i<a->skeleton.size();i++)
		{
			Sprite *b = a->skeleton[i];

			//name
			fileout << b->getName() << std::endl;

			//first row
			fileout
				<< b->att_angle.size()
				<< " " << (int)b->getRotateX()
				<< " " << (int)b->getRotateY()
				<< " " << fixed << setprecision(2) << (double)b->getScaleX()
				<< " " << fixed << setprecision(2) << (double)b->getScaleY()
				<< " " << b->getImage()->getFilePath()
				<< " " << b->getText()
				<< std::endl;
			//zadnja dva broja spremljena u get text, to treba izminit
			//ne moze preko state, id jer to remeti uigid, koji ne sacuva one kod kojih je sve -999

			//parent
			fileout << (bool)b->parent;

			if(b->parent) fileout << " " << b->parent->getName();
			else fileout << " " << "#";

			fileout << " " << b->parent_att_dot << std::endl;

			//attachment positions
			for(unsigned int i=0;i<b->att_angle.size();i++)
			{
				fileout << (int)(b->att_posx[i]) << " " << (int)(b->att_posy[i]) << std::endl;
			}
		}

		fileout.close();

	}

}//namespace