class CreateCards < ActiveRecord::Migration[5.1]
  def change
    create_table :cards do |t|
      t.string :sid
      t.float :money
      t.integer :flag

      t.timestamps
    end
  end
end
