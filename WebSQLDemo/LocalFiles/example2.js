/*
* @file example2.js
* @author Mikael Kindborg
*
* Short example that shows how to use the small wrapper library
* in file database.js.
*
* The actual example is the same as in example1.js.
*
* View output in the console log, e.g. logcat on Android.
*/

function DatabaseTest2()
{
	console.log("Running DatabaseTest2");
	var db = database("test.db")
	db.execSQL("DROP TABLE IF EXISTS pet");
	db.execSQL("CREATE TABLE pet (name, age, curiosity)");
	db.execSQL("INSERT INTO pet VALUES (?, ?, ?)", ["Charmy", 7, 0.6]);
	db.execSQL("INSERT INTO pet VALUES (?, ?, ?)", ["Sporty", 4, 0.9]);
	db.execSQL("INSERT INTO pet VALUES (?, ?, ?)", ["Rookie", 2, 0.4]);
	db.execSQL("UPDATE pet SET name=? WHERE name=?", ["Snoopy", "Sporty"]);
	db.execSQL("DELETE FROM pet WHERE name=?", ["Rookie"]);
	db.execSQL("SELECT COUNT(*) FROM (SELECT * FROM pet)", [],
		function(resultSet)
		{
			var row = resultSet.rows.item(0);
			console.log("Number of rows in database: " + row["COUNT(*)"]);
		});
	db.execSQL("SELECT * FROM pet", [],
		function(resultSet)
		{
			console.log("All rows:");
			for (var i = 0; i < resultSet.rows.length; ++i)
			{
				var row = resultSet.rows.item(i);
				console.log("  " + row.name + " " + row.age + " " + row.curiosity);
			}
		});
}
