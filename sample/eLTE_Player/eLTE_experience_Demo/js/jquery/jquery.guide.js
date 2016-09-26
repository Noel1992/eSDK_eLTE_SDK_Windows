
(function($) {
	$.extend({
		wlGuideBox: {
			def: {
				title: "Current Step",
				text: "This is guide text"
			},
			name: "D" + new Date().getTime(),
			g: new Array(),
			gn: 0,
			s: 0,
			created: false,
			create: function() {
				var the = this;
				the.created = true;
				the.ele = $("<div id='wlGuide_box" + the.name + "' class='cGuide_box'><span class='cGuide_arrow'></span></div>");
				the.blank = {
					t: $("<div class='cGuide_blank_t'></div>"),
					r: $("<div class='cGuide_blank_r'></div>"),
					b: $("<div class='cGuide_blank_b'></div>"),
					l: $("<div class='cGuide_blank_l'></div>")
				};
				the.blank_all = $("<div class='cGuide_blank'></div>").append(the.blank.t, the.blank.r, the.blank.b, the.blank.l);
				the.content = $("<div class='cGuide_content'></div>");
				the.title = $("<h2 class='cGuide_title'></h2>");
				the.scount = $("<span class='cGuide_scount'></span>");
				the.text = $("<p></p>");
				the.button = {
					p: $("<a href='javascript:void(0)' class='cGuide_button'>Previous</a>'").click(function() {
						if (!$(this).hasClass("cGuide_button_unable")) {
							the.step( - 1);
						}
					}),
					n: $("<a href='javascript:void(0)' class='cGuide_button'>Next</a>'").click(function() {
						if (!$(this).hasClass("cGuide_button_unable")) {
							the.step(1);
						}
					}),
					c: $("<a href='javascript:void(0)' class='cGuide_button'>Close</a>'").click(function() {
						if (!$(this).hasClass("cGuide_button_unable")) {
							the.close();
						}
					})
				};
				$("body").prepend(
					the.blank_all, 
					the.ele.append(
						the.content.append(
							the.title, the.scount, $("<div  class='cGuide_content_text'></div>").append(the.text), 
							the.button.p, the.button.n, the.button.c
						)
					)
				);
			},
			step: function(i) {
				var the = this;
				var ind = the.s += i;
				if (ind > the.gn) {
					ind = the.s = the.gn;
					return false;
				} else if (ind < 1) {
					ind = the.s = 1;
					return false;
				}
				the.button.p.attr("class", ind === 1 ? "cGuide_button_unable": "cGuide_button");
				the.button.n.attr("class", ind === the.gn ? "cGuide_button_unable": "cGuide_button");
				the.button.c.attr("class", ind === the.gn ? "cGuide_button": "cGuide_button");
				if (the.g[ind]) {
					the.ele.hide();
					the.title.html(the.g[ind].title);
					the.scount.html("(" + ind + "/" + the.gn + ")");
					the.text.html(the.g[ind].text);
					var obj = $(the.g[ind].ele);
					var bfs = {
						left: 0,
						top: 0
					};
					var ofs = obj.offset();
					if ($.browser.msie && $.browser.version == "6.0") {
						var bfs = $("body").offset()
					}
					var h = obj.outerHeight();
					var w = obj.outerWidth();
					var wc = Array($(document).width(), $(document).height());
					var sc = {
						left: obj.scrollLeft(),
						top: obj.scrollTop()
					};
					the.ele.css({
						left: ofs.left,
						top: ofs.top + h + 15
					});
					the.blank.t.css({
						width: wc[0],
						height: ofs.top - 1,
						left: 0,
						top: 0
					});
					the.blank.r.css({
						height: h + 2,
						width: wc[0] - ofs.left - w - 1,
						left: ofs.left + w + 1,
						top: ofs.top - 1 + sc.top
					});
					var bHeight = wc[1] - ofs.top - h - 1;
					bHeight = bHeight > the.ele.outerHeight() + 20 ? bHeight: the.ele.outerHeight() + 20;
					the.blank.b.css({
						width: wc[0],
						height: bHeight,
						left: 0,
						top: ofs.top + h + 1 + sc.top
					});
					the.blank.l.css({
						height: h + 2,
						width: ofs.left - 1,
						left: 0,
						top: ofs.top - 1 + sc.top
					});
					var toTop = 0;
					var toBottom = ofs.top + the.ele.outerHeight() + 20;
					while (toBottom - toTop > $(window).height()) {
						toTop += 200;
					}
					$("html,body").animate({
						scrollTop: toTop
					},
					200);
					the.ele.show();
				}
			},
			show: function(gs) {
				if (!this.created) {
					this.create();
				}
				var the = this;
				the.blank_all.show().find("div").css({
					opacity: "0.5"
				});
				the.ele.fadeIn();
				the.s = the.gn = 0;
				the.g = new Array();
				$.each(gs,
				function(i, e) {
					the.g[i + 1] = $.extend({},
					the.def, e);
					the.gn++;
				});
				the.step(1);
			},
			close: function() {
				var the = this;
				the.ele.hide();
				the.blank_all.hide();
			}
		},
		wlGuide: function(gs) {
			if (gs && gs.close) {
				$.wlGuideBox.close();
				return true;
			}
			return $.wlGuideBox.show(gs);
		}
	})
})(jQuery);