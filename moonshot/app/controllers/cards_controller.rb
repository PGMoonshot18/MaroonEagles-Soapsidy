class CardsController < ApplicationController
  before_action :set_card, only: [:show, :edit, :update, :destroy]

  # GET /cards
  # GET /cards.json
  def index
    @cards = Card.all
  end

  # GET /cards/1
  # GET /cards/1.json
  def show
  end

  # GET /cards/new
  def new
    @card = Card.new
  end

  # GET /cards/1/edit
  def edit
  end

  # POST /cards
  # POST /cards.json
  def create
    @card = Card.new(card_params)

    respond_to do |format|
      if @card.save
        format.html { redirect_to @card, notice: 'Card was successfully created.' }
        format.json { render :show, status: :created, location: @card }
      else
        format.html { render :new }
        format.json { render json: @card.errors, status: :unprocessable_entity }
      end
    end
  end

  # GET /cards/:sid
  def get_sid
    @card = Card.where(sid: params[:sid]).first.flag
    respond_to do |format|
      format.json  { render json: @card }
    end
  end

  # POST /cards/pump
  def post_sid
    @card = Card.where(sid: sid_params).first
    @res = {status: "failure"} # flag == 0
    if @card.flag == 1
      @card.flag = 0
      @card.pump += 1
      @res = {status: "success"}
    end
    respond_to do |format|
      if @card.save
        #format.html { redirect_to @card, notice: 'Card was successfully created.' }
        format.json { render json: @res }
      else
        # format.html { render :new }
        @res[:status] = "error"
        format.json { render json: @res }
      end
    end
  end

  # PATCH/PUT /cards/1
  # PATCH/PUT /cards/1.json
  def update
    respond_to do |format|
      if @card.update(card_params)
        format.html { redirect_to @card, notice: 'Card was successfully updated.' }
        format.json { render :show, status: :ok, location: @card }
      else
        format.html { render :edit }
        format.json { render json: @card.errors, status: :unprocessable_entity }
      end
    end
  end

  # DELETE /cards/1
  # DELETE /cards/1.json
  def destroy
    @card.destroy
    respond_to do |format|
      format.html { redirect_to cards_url, notice: 'Card was successfully destroyed.' }
      format.json { head :no_content }
    end
  end

  private
    # Use callbacks to share common setup or constraints between actions.
    def set_card
      @card = Card.find(params[:id])
    end

    # Never trust parameters from the scary internet, only allow the white list through.
    def card_params
      params.require(:card).permit(:sid, :money, :flag, :pump)
    end

    def sid_params
      params.require(:sid)
    end
end
