#include "Advanced2D.h"

namespace Advanced2D
{
	EditSprite::EditSprite()
	{
		this->setNULL();
	}

	EditSprite::~EditSprite(){}

	void EditSprite::setNULL()
	{
		this->main_form = NULL;

		this->pos_x = NULL;
		this->pos_y = NULL;
		this->size_x = NULL;
		this->size_y = NULL;
		this->collidable = NULL;

		this->scale_x = NULL;
		this->scale_y = NULL;

		this->rotate = NULL;
		this->render_type = NULL;
		this->scroll = NULL;
		this->offset_mx = NULL;
		this->offset_my = NULL;
		this->flip_v = NULL;
		this->flip_h = NULL;
		this->frame_relx = NULL;
		this->frame_rely = NULL;

		this->my_class = NULL;
		this->type = NULL;
		this->id = NULL;
		this->state = NULL;

		this->vel_x = NULL;
		this->vel_y = NULL;

		this->energy = NULL;
		this->difficulty = NULL;

		this->command = NULL;

		this->b_alpha = NULL;
		this->b_red = NULL;
		this->b_green = NULL;
		this->b_blue = NULL;
	}

	void EditSprite::Init()
	{
		Button *tile = new Button();
		tile->setType(FORM_AUTOHIDE);
		tile->setImage(g_engine->gui->default_t);
		tile->setText("Sp");
		tile->setColor_Texture(0xFF888888);
		tile->setSize(32,48);
		tile->setPosition(2,28);		
		g_engine->gui->addButton(tile);

		Button *ee_form = g_engine->gui->form_add(
			" EDIT SPRITE",
			tile->getX()+tile->getWidth(),tile->getY(),
			225,g_engine->getScreenHeight()-tile->getY(),'L',false);

		ee_form->form[0]->setText("O");
		ee_form->form[0]->clickable=false;
		ee_form->form[0]->setCollidable(false);
		tile->child=ee_form;

		//mozda ni netreba
		this->main_form = ee_form;

		//drawing
		ee_form->form[1]->row_height = g_engine->e_font->size+2;
		ee_form->form[1]->row_x=2;
		ee_form->form[1]->row_y=2;

		//button positions and stuff
		int width = 50;
		int height = g_engine->e_font->size;
		int row_distance = 2;

		int posx = ee_form->getX() + 2 + 150;
		int posy = ee_form->form[1]->getY()+2;

		this->pos_x = new Button();
		this->pos_x->setType(POS_X);
		this->pos_x->setImage(g_engine->gui->default_t);
		this->pos_x->setSize(width,height);
		this->pos_x->setPosition(posx,posy);

		this->pos_y = new Button();
		this->pos_y->setType(POS_Y);
		this->pos_y->setImage(g_engine->gui->default_t);
		this->pos_y->setSize(width,height);
		this->pos_y->setPosition(this->pos_x->getX()+this->pos_x->getWidth()+2,posy);

		posy += height + row_distance;

		this->size_x = new Button();
		this->size_x->setType(SIZE_X);
		this->size_x->setImage(g_engine->gui->default_t);
		this->size_x->setSize(width,height);
		this->size_x->setPosition(posx,posy);

		this->size_y = new Button();
		this->size_y->setType(SIZE_Y);
		this->size_y->setImage(g_engine->gui->default_t);
		this->size_y->setSize(width,height);
		this->size_y->setPosition(this->size_x->getX()+this->size_x->getWidth()+2,posy);

		posy += height + row_distance;

		this->collidable = g_engine->gui->add_checkbox(
			COLLIDABLE,
			posx,posy,
			20,g_engine->e_font->size,"Y","N",NULL,1);

		posy += height + row_distance;		

		this->scale_x = new Button();
		this->scale_x->setType(SCALE_X);
		this->scale_x->setImage(g_engine->gui->default_t);
		this->scale_x->setSize(width,height);
		this->scale_x->setPosition(this->collidable->getX(),posy);

		this->scale_y = new Button();
		this->scale_y->setType(SCALE_Y);
		this->scale_y->setImage(g_engine->gui->default_t);
		this->scale_y->setSize(width,height);
		this->scale_y->setPosition(this->scale_x->getX()+this->scale_x->getWidth()+2,posy);

		posy += height + row_distance;

		this->rotate = new Button();
		this->rotate->setType(ROTATE);
		this->rotate->setImage(g_engine->gui->default_t);
		this->rotate->setSize(width,height);
		this->rotate->setPosition(posx,posy);

		posy += height + row_distance;

		this->flip_h = g_engine->gui->add_checkbox(
			B_FLIPH,
			posx,posy,
			20,g_engine->e_font->size,"Y","N",NULL,0);

		this->flip_v = g_engine->gui->add_checkbox(
			B_FLIPV,
			flip_h->getX()+flip_h->getWidth()+2,posy,
			20,g_engine->e_font->size,"Y","N",NULL,0);

		posy += height + row_distance;

		this->scroll = new Button();
		this->scroll->setType(SCROLL);
		this->scroll->setImage(g_engine->gui->default_t);
		this->scroll->setSize(width,height);
		this->scroll->setPosition(posx,posy);		

		posy += height + row_distance;

		this->offset_mx = new Button();
		this->offset_mx->setType(B_OFFSET_MX);
		this->offset_mx->setImage(g_engine->gui->default_t);
		this->offset_mx->setSize(width,height);
		this->offset_mx->setPosition(posx,posy);

		this->offset_my = new Button();
		this->offset_my->setType(B_OFFSET_MY);
		this->offset_my->setImage(g_engine->gui->default_t);
		this->offset_my->setSize(width,height);
		this->offset_my->setPosition(this->offset_mx->getX()+this->offset_mx->getWidth()+2,posy);		

		posy += height + row_distance;

		std::vector <string> render_type;		
		render_type.push_back("normal");
		render_type.push_back("tiled");
		render_type.push_back("scroll_x");	
		render_type.push_back("scroll_y");	
		render_type.push_back("tilescroller_x");	
		render_type.push_back("tilescroller_y");	
		render_type.push_back("stretch");
		render_type.push_back("rel_frame");
		render_type.push_back("shader");

		this->render_type = g_engine->gui->add_dropbox(
			RENDER_TYPE,
			posx,posy,
			80,g_engine->e_font->size,
			render_type);	

		posy += height + row_distance;

		this->my_class = new Button();
		this->my_class->setType(MY_CLASS);
		this->my_class->setImage(g_engine->gui->default_t);
		this->my_class->setSize(width,height);
		this->my_class->setPosition(posx,posy);		

		posy += height + row_distance;

		this->type = new Button();
		this->type->setType(TYPE);
		this->type->setImage(g_engine->gui->default_t);
		this->type->setSize(width,height);
		this->type->setPosition(posx,posy);

		posy += height + row_distance;

		this->id = new Button();
		this->id->setType(ID);
		this->id->setImage(g_engine->gui->default_t);
		this->id->setSize(width,height);
		this->id->setPosition(posx,posy);

		posy += height + row_distance;

		this->state = new Button();
		this->state->setType(STATE);
		this->state->setImage(g_engine->gui->default_t);
		this->state->setSize(width,height);
		this->state->setPosition(posx,posy);

		posy += height + row_distance;

		this->vel_x = new Button();
		this->vel_x->setType(VEL_X);
		this->vel_x->setImage(g_engine->gui->default_t);
		this->vel_x->setSize(width,height);
		this->vel_x->setPosition(posx,posy);

		this->vel_y = new Button();
		this->vel_y->setType(VEL_Y);
		this->vel_y->setImage(g_engine->gui->default_t);
		this->vel_y->setSize(width,height);
		this->vel_y->setPosition(this->vel_x->getX()+this->vel_x->getWidth()+2,posy);

		posy += height + row_distance;
		
		this->frame_relx = new Button();
		this->frame_relx->setType(B_FRAME_RELX);
		this->frame_relx->setImage(g_engine->gui->default_t);
		this->frame_relx->setSize(width,height);
		this->frame_relx->setPosition(posx,posy);

		this->frame_rely = new Button();
		this->frame_rely->setType(B_FRAME_RELY);
		this->frame_rely->setImage(g_engine->gui->default_t);
		this->frame_rely->setSize(width,height);
		this->frame_rely->setPosition(this->frame_relx->getX()+this->frame_relx->getWidth()+2,posy);

		posy += height + row_distance;		

		this->energy = new Button();
		this->energy->setType(B_ENERGY);
		this->energy->setImage(g_engine->gui->default_t);
		this->energy->setSize(width,height);
		this->energy->setPosition(posx,posy);

		posy += height + row_distance;		

		this->difficulty = new Button();
		this->difficulty->setType(B_DIFFICULTY);
		this->difficulty->setImage(g_engine->gui->default_t);
		this->difficulty->setSize(width,height);
		this->difficulty->setPosition(posx,posy);

		posy += 2*height + row_distance;

		//COLOR
		this->b_alpha = g_engine->gui->add_slider_h(posx-50,posy,250,g_engine->e_font->size,0,255);
		this->b_alpha->setType(B_ALPHA);

		posy += height + 2*row_distance;

		this->b_red = g_engine->gui->add_slider_h(posx-50,posy,250,g_engine->e_font->size,0,255);
		b_red->form[1]->setColor_Texture(0xFFDD0000);
		this->b_red->setType(B_RED);

		posy += height + 2*row_distance;

		this->b_green = g_engine->gui->add_slider_h(posx-50,posy,250,g_engine->e_font->size,0,255);
		b_green->form[1]->setColor_Texture(0xFF00DD00);
		this->b_green->setType(B_GREEN);

		posy += height + 2*row_distance;

		this->b_blue = g_engine->gui->add_slider_h(posx-50,posy,250,g_engine->e_font->size,0,255);
		b_blue->form[1]->setColor_Texture(0xFF0000DD);
		this->b_blue->setType(B_BLUE);		

		posy += 3*height + row_distance;		

		this->command = new Button();
		this->command->setType(SET_TEXT);
		this->command->setImage(g_engine->gui->default_t);
		this->command->setSize(7*width,height);
		this->command->setPosition(ee_form->form[1]->getX() + 2*2,posy);
		this->command->text_positioning = 'L';

		ee_form->children.push_back(this->pos_x);
		ee_form->children.push_back(this->pos_y);
		ee_form->children.push_back(this->size_x);
		ee_form->children.push_back(this->size_y);
		ee_form->children.push_back(this->collidable);
		ee_form->children.push_back(this->scale_x);		
		ee_form->children.push_back(this->scale_y);
		ee_form->children.push_back(this->rotate);
		ee_form->children.push_back(this->flip_h);
		ee_form->children.push_back(this->flip_v);
		ee_form->children.push_back(this->render_type);
		ee_form->children.push_back(this->render_type->form[0]);//add arrow	too	
		ee_form->children.push_back(this->scroll);
		ee_form->children.push_back(this->offset_mx);
		ee_form->children.push_back(this->offset_my);
		ee_form->children.push_back(this->my_class);
		ee_form->children.push_back(this->type);
		ee_form->children.push_back(this->id);	
		ee_form->children.push_back(this->state);
		ee_form->children.push_back(this->vel_x);
		ee_form->children.push_back(this->vel_y);
		ee_form->children.push_back(this->frame_relx);
		ee_form->children.push_back(this->frame_rely);
		ee_form->children.push_back(this->energy);
		ee_form->children.push_back(this->difficulty);
		ee_form->children.push_back(this->command);

		for(int i=0;i<ee_form->children.size();i++)
		{
			ee_form->children[i]->setGUI(EDITSPRITE_CLASS);

			if (ee_form->children[i]->getClass()!=CHECKBOX && ee_form->children[i]->getClass()!=DROPBOX
				&& ee_form->children[i]->getClass()!=DROPBOX_OPTION && ee_form->children[i]->getClass()!=DROPBOX_ARROW)
			{
				ee_form->children[i]->setClass(TEXTBOX);
				ee_form->children[i]->setColor_Texture(0xFFFFFFFF);							
				ee_form->children[i]->setColor_Text(0xFF000000);				
				g_engine->gui->addButton(ee_form->children[i]);
			}
		}

		//da preskoci pretvaranje u textbox
		ee_form->children.push_back(this->b_alpha);
		ee_form->children.push_back(this->b_red);
		ee_form->children.push_back(this->b_green);
		ee_form->children.push_back(this->b_blue);
		for(int i=0;i<b_alpha->form.size();i++)
		{
			ee_form->children.push_back(b_alpha->form[i]);
			ee_form->children.push_back(b_red->form[i]);
			ee_form->children.push_back(b_green->form[i]);
			ee_form->children.push_back(b_blue->form[i]);
		}

		for(int i=0;i<ee_form->children.size();i++)
		{
			ee_form->children[i]->setGUI(EDITSPRITE_CLASS);
		}

		ee_form->form[1]->text_positioning = 'L';
		ee_form->form[1]->page.push_back(" pos[x,y]");
		ee_form->form[1]->page.push_back(" size[w,h]");
		ee_form->form[1]->page.push_back(" collidable");
		ee_form->form[1]->page.push_back(" scale[x,y]");
		ee_form->form[1]->page.push_back(" rotate[°]");
		ee_form->form[1]->page.push_back(" flip[h,v]");
		ee_form->form[1]->page.push_back(" scroll");
		ee_form->form[1]->page.push_back(" offset[x,y]");
		ee_form->form[1]->page.push_back(" render_type");
		ee_form->form[1]->page.push_back(" class[num]");
		ee_form->form[1]->page.push_back(" type[num]");
		ee_form->form[1]->page.push_back(" id[num]");
		ee_form->form[1]->page.push_back(" state[num]");
		ee_form->form[1]->page.push_back(" vel[x,y]");
		ee_form->form[1]->page.push_back(" frame_rel[x,y]");
		ee_form->form[1]->page.push_back(" energy");
		ee_form->form[1]->page.push_back(" difficulty");
		ee_form->form[1]->page.push_back("");
		ee_form->form[1]->page.push_back(" ARGB");
		ee_form->form[1]->page.push_back("");
		ee_form->form[1]->page.push_back("");
		ee_form->form[1]->page.push_back("");
		ee_form->form[1]->page.push_back("");
		ee_form->form[1]->page.push_back(" command");

		//fit panel(children based)
		g_engine->gui->form_fitpanel_w(ee_form);
		//g_engine->gui->form_fitpanel_h(ee_form);

		//UPDATE VISIBILITY
		ee_form->setVisible(false);
		for(int i=0;i<ee_form->form.size();i++)
		{
			ee_form->form[i]->setVisible(false);
		}
		for(int i=0;i<ee_form->children.size();i++)
		{
			ee_form->children[i]->setVisible(false);
		}
	}	

	void EditSprite::Update_form(Sprite *a)
	{
		std::stringstream temp;

		if (a!=NULL)
		{
			this->pos_x->setText(g_engine->inttostring(a->getX()));
			this->pos_y->setText(g_engine->inttostring(a->getY()));
			this->size_x->setText(g_engine->inttostring(a->getWidth()));//ovo treba standardizirat /posebno za frame posebno za sprite
			this->size_y->setText(g_engine->inttostring(a->getHeight()));
			this->scale_x->setText(g_engine->inttostring(a->getScaleX()));
			this->scale_y->setText(g_engine->inttostring(a->getScaleY()));
			this->rotate->setText(g_engine->inttostring(g_engine->math->toDegrees(a->getRotation())));
			this->my_class->setText(g_engine->inttostring(a->getClass()));
			this->type->setText(g_engine->inttostring(a->getType()));
			this->id->setText(g_engine->inttostring(a->getID()));
			this->state->setText(g_engine->inttostring(a->getState()));
			this->vel_x->setText(g_engine->inttostring(a->getVelocityX()));
			this->vel_y->setText(g_engine->inttostring(a->getVelocityY()));
			this->frame_relx->setText(g_engine->inttostring(a->getFrameRelX()));
			this->frame_rely->setText(g_engine->inttostring(a->getFrameRelY()));
			this->energy->setText(g_engine->inttostring(a->energy));
			this->difficulty->setText(g_engine->inttostring(a->difficulty));
			this->command->setText(a->getText());
			//scroller
			if(a->getRenderType()==SCROLLER_X || a->getRenderType()==TILESCROLLER_X)
				this->scroll->setText(g_engine->inttostring(a->getScrollStepX()));
			else if(a->getRenderType()==SCROLLER_Y || a->getRenderType()==TILESCROLLER_Y)
				this->scroll->setText(g_engine->inttostring(a->getScrollStepY()));
			else
				this->scroll->setText(g_engine->inttostring(0));
			this->offset_mx->setText(g_engine->inttostring(a->getOffsetMX()));
			this->offset_my->setText(g_engine->inttostring(a->getOffsetMY()));
			//COLOR
			this->b_alpha->setText(g_engine->inttostring(g_engine->ar_graphics->getAlpha(a->getColor_Texture())));
			this->b_red->setText(g_engine->inttostring(g_engine->ar_graphics->getRed(a->getColor_Texture())));
			this->b_green->setText(g_engine->inttostring(g_engine->ar_graphics->getGreen(a->getColor_Texture())));
			this->b_blue->setText(g_engine->inttostring(g_engine->ar_graphics->getBlue(a->getColor_Texture())));
			g_engine->gui->update_slider_h_value(
				this->b_alpha,g_engine->ar_graphics->getAlpha(a->getColor_Texture()));
			g_engine->gui->update_slider_h_value(
				this->b_red,g_engine->ar_graphics->getRed(a->getColor_Texture()));
			g_engine->gui->update_slider_h_value(
				this->b_green,g_engine->ar_graphics->getGreen(a->getColor_Texture()));
			g_engine->gui->update_slider_h_value(
				this->b_blue,g_engine->ar_graphics->getBlue(a->getColor_Texture()));

			//dropbox
			if (a->getRenderType()==NORMAL) this->render_type->setText("normal");
			else if (a->getRenderType()==TILE) this->render_type->setText("tiled");
			else if (a->getRenderType()==SCROLLER_X) this->render_type->setText("scroll_x");
			else if (a->getRenderType()==SCROLLER_Y) this->render_type->setText("scroll_y");
			else if (a->getRenderType()==TILESCROLLER_X) this->render_type->setText("tilescroller_x");
			else if (a->getRenderType()==TILESCROLLER_Y) this->render_type->setText("tilescroller_y");			
			else if (a->getRenderType()==STRETCH) this->render_type->setText("stretch");
			else if (a->getRenderType()==REL_FRAME)	this->render_type->setText("rel_frame");
			else if (a->getRenderType()==SHADERSPRITE) this->render_type->setText("shader");

			if (a->getCollidable()==1)
			{
				this->collidable->setState(1);
				this->collidable->setText("Y");
				this->collidable->setColor_Texture(0xFF00FF00);
			}
			else
			{
				this->collidable->setState(0);
				this->collidable->setText("N");
				this->collidable->setColor_Texture(0xFFFF0000);
			}

			if (a->flip_h)
			{
				this->flip_h->setState(1);
				this->flip_h->setText("Y");
				this->flip_h->setColor_Texture(0xFF00FF00);
			}
			else
			{
				this->flip_h->setState(0);
				this->flip_h->setText("N");
				this->flip_h->setColor_Texture(0xFFFF0000);
			}

			if (a->flip_v)
			{
				this->flip_v->setState(1);
				this->flip_v->setText("Y");
				this->flip_v->setColor_Texture(0xFF00FF00);
			}
			else
			{
				this->flip_v->setState(0);
				this->flip_v->setText("N");
				this->flip_v->setColor_Texture(0xFFFF0000);
			}			
		}
		else//a==NULL
		{}

	}//Update()

	void EditSprite::Update_sprite(Sprite *a,Button *button)
	{
		switch(button->getType())
		{
		case POS_X:
			a->setX((int)g_engine->stringtoint(button->getText()));			
			break;

		case POS_Y:
			a->setY((int)g_engine->stringtoint(button->getText()));
			break;

		case SIZE_X:
			a->setWidth((int)g_engine->stringtoint(button->getText()));			
			break;

		case SIZE_Y:
			a->setHeight((int)g_engine->stringtoint(button->getText()));
			break;

		case SCALE_X:
			a->setScaleX(g_engine->stringtoint(button->getText()));
			break;

		case SCALE_Y:
			a->setScaleY(g_engine->stringtoint(button->getText()));
			break;

		case ROTATE:
			a->setRotation(g_engine->math->toRadians((int)g_engine->stringtoint(button->getText())));
			break;

		case SCROLL:
			if(a->getRenderType()==SCROLLER_X || a->getRenderType()==TILESCROLLER_X)
				a->setScrollStepX(g_engine->stringtoint(button->getText()));
			else if(a->getRenderType()==SCROLLER_Y || a->getRenderType()==TILESCROLLER_Y)
				a->setScrollStepY(g_engine->stringtoint(button->getText()));
			break;	

		case B_OFFSET_MX:
			a->setOffsetMX(g_engine->stringtoint(button->getText()));
			break;

		case B_OFFSET_MY:
			a->setOffsetMY(g_engine->stringtoint(button->getText()));
			break;

		case MY_CLASS:
			a->setClass((int)g_engine->stringtoint(button->getText()));
			break;

		case TYPE:
			a->setType((int)g_engine->stringtoint(button->getText()));
			break;

		case ID:
			a->setID((int)g_engine->stringtoint(button->getText()));
			break;

		case STATE:
			a->setState((int)g_engine->stringtoint(button->getText()));
			break;

		case VEL_X:
			a->setVelocityX(g_engine->stringtoint(button->getText()));
			break;

		case VEL_Y:
			a->setVelocityY(g_engine->stringtoint(button->getText()));
			break;
		
		case B_FRAME_RELX:
			a->setFrameRelX(g_engine->stringtoint(button->getText()));
			break;	

		case B_FRAME_RELY:
			a->setFrameRelY(g_engine->stringtoint(button->getText()));
			break;	

		case SET_TEXT:
			a->setText(button->getText());
			break;

		case B_ENERGY:
			a->energy = g_engine->stringtoint(button->getText());
			break;

		case B_DIFFICULTY:
			a->difficulty = g_engine->stringtoint(button->getText());
			break;
		}

		this->Update_form(a);

	}

	void EditSprite::ButtonPress(Button *button,int mousebutton)
	{
		if(g_engine->gui->getSelectedSprite()==NULL) return;
		Sprite *a = g_engine->gui->getSelectedSprite();	
		
		switch (mousebutton)
		{
		case 0:
			switch (button->getType())
			{
			case B_ALPHA:
				a->setColor_Texture(D3DCOLOR_ARGB(
					(int)g_engine->stringtoint(button->getText()),
					g_engine->ar_graphics->getRed(a->getColor_Texture()),				
					g_engine->ar_graphics->getGreen(a->getColor_Texture()),
					g_engine->ar_graphics->getBlue(a->getColor_Texture())));
				break;

			case B_RED:
				a->setColor_Texture(D3DCOLOR_ARGB(
					g_engine->ar_graphics->getAlpha(a->getColor_Texture()),
					(int)g_engine->stringtoint(button->getText()),
					g_engine->ar_graphics->getGreen(a->getColor_Texture()),
					g_engine->ar_graphics->getBlue(a->getColor_Texture())));
				break;

			case B_GREEN:
				a->setColor_Texture(D3DCOLOR_ARGB(
					g_engine->ar_graphics->getAlpha(a->getColor_Texture()),
					g_engine->ar_graphics->getRed(a->getColor_Texture()),
					(int)g_engine->stringtoint(button->getText()),
					g_engine->ar_graphics->getBlue(a->getColor_Texture())));
				break;

			case B_BLUE:
				a->setColor_Texture(D3DCOLOR_ARGB(
					g_engine->ar_graphics->getAlpha(a->getColor_Texture()),
					g_engine->ar_graphics->getRed(a->getColor_Texture()),
					g_engine->ar_graphics->getGreen(a->getColor_Texture()),
					(int)g_engine->stringtoint(button->getText())));
				break;
			}
		}
	}

	void EditSprite::ButtonRelease(Button *button,int mousebutton)
	{
		if(g_engine->gui->getSelectedSprite()==NULL) return;
		Sprite *a = g_engine->gui->getSelectedSprite();	

		switch (mousebutton)
		{
		case 0:
			switch (button->getType())
			{
			case RENDER_TYPE:
				if (button->getText()=="normal" && a->getRenderType()!=NORMAL)
				{
					a->setSize(
						a->getImage()->tile_width,
						a->getImage()->tile_height);
					a->setRenderType(NORMAL);						
				}
				else if (button->getText()=="tiled"  && a->getRenderType()!=TILE)
				{
					a->setRenderType(TILE);
				}
				else if (button->getText()=="scroll_x")
					a->setRenderType(SCROLLER_X);
				else if (button->getText()=="scroll_y")
					a->setRenderType(SCROLLER_Y);
				else if (button->getText()=="tilescroller_x")
					a->setRenderType(TILESCROLLER_X);
				else if (button->getText()=="tilescroller_y")
					a->setRenderType(TILESCROLLER_Y);
				else if (button->getText()=="stretch")
					a->setRenderType(STRETCH);
				else if (button->getText()=="rel_frame")
					a->setRenderType(REL_FRAME);
				else if (button->getText()=="shader")
					a->setRenderType(SHADERSPRITE);
				break;

			case COLLIDABLE:
				a->setCollidable(button->getState());					
				this->Update_form(a);
				break;

			case B_FLIPH:
				a->flip_h = button->getState();					
				this->Update_form(a);					
				break;

			case B_FLIPV:
				a->flip_v = button->getState();					
				this->Update_form(a);					
				break;
			}
		}

	}//ButtonRelease()

}//Advanced2D