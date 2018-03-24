Rails.application.routes.draw do
  resources :cards
  get '/cards/sid/:sid/' => 'cards#get_sid'
  post '/cards/pump/' => 'cards#post_sid'
  post '/cards/soap/' => 'cards#post_sid_soap'
  # For details on the DSL available within this file, see http://guides.rubyonrails.org/routing.html
end
