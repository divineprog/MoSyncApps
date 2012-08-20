/*
* @file example1.js
* @author Mikael Kindborg
*
* Example that shows how to use the Web SQL Database API.
*
* View output in the console log, e.g. logcat on Android.
*/

function DatabaseTest1()
{
	console.log("Running DatabaseTest1");
	var db = openDatabase("test.db", "1", "test.db", 65536);
	db.transaction(
		function(tx)
		{
			tx.executeSql("DROP TABLE IF EXISTS pet");
			tx.executeSql("CREATE TABLE pet (name, age, curiosity)");
			tx.executeSql("INSERT INTO pet VALUES (?, ?, ?)", ["Charmy", 7, 0.6]);
			tx.executeSql("INSERT INTO pet VALUES (?, ?, ?)", ["Sporty", 4, 0.9]);
			tx.executeSql("INSERT INTO pet VALUES (?, ?, ?)", ["Rookie", 2, 0.4]);
			tx.executeSql("UPDATE pet SET name=? WHERE name=?", ["Snoopy", "Sporty"]);
			tx.executeSql("DELETE FROM pet WHERE name=?", ["Rookie"]);
			tx.executeSql("SELECT COUNT(*) FROM (SELECT * FROM pet)", [],
				function(tx, result)
				{
					var row = result.rows.item(0);
					console.log("Number of rows in database: " + row["COUNT(*)"]);
				});
			tx.executeSql("SELECT * FROM pet", [],
				function(tx, result)
				{
					console.log("All rows:");
					for (var i = 0; i < result.rows.length; ++i)
					{
						var row = result.rows.item(i);
						console.log("  " + row.name + " " + row.age + " " + row.curiosity);
					}
				});
		},
        function(error) {
            console.log("Transaction Error: " + error.message); },
        function() {
            console.log("Transaction Success");
            // Run next test.
            DatabaseTest2(); }
	);
}
