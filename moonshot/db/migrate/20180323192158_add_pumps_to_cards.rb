class AddPumpsToCards < ActiveRecord::Migration[5.1]
  def change
    add_column :cards, :pump, :integer
  end
end
