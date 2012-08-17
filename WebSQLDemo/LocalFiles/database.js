/**
 * Object that wraps database functions.
 */
var database = function(name)
{
	// Helper function used internally.
	function open(name)
	{
		try
		{
		    if (!window.openDatabase)
		    {
		        return null;
		    }
		    else
		    {
		    	return openDatabase(name, "1", name, 65536);
		    }
		}
		catch(e)
		{
		    return null;
		}
	}

	// Create the database.
	var db = open(name);

	// Add the execSQL function.
    db.execSQL = function(sql, params, successFun, errorFun)
    {
		if (!params)
		{
			params = [];
		}

		if (!successFun)
		{
			// Default result function reports errors.
			successFun = function(result)
			{
			};
		}

		if (!errorFun)
		{
			// Default result function reports errors.
			errorFun = function(error)
			{
				// TODO: Uncomment if you want error messages.
				//alert("DB Error: " + error.message);
			};
		}

	    db.transaction(
	        function(tx)
	        {
	        	tx.executeSql(
					sql,
					params,
					function(tx, result) {
						successFun(result); },
					function(tx, error) {
						errorFun(error); });
	        });
    };

    return db;
};
