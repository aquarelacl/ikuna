//jQuery wrapper
(function ($) {

	$(document).ready(

		function(){
			try
			{
				$("body.not-logged-in .vud-widget-updown div.updown-up.up-active").click(
					function()
					{
						window.location.href = "/user/login?destination=ideas";
					}
					);
			
			}catch(e){
				console.log(e);
			}

		}

		);
})(jQuery);